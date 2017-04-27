// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "EnemyAI/Spawning/EnemyGate.h"
#include "EnemyAI/EnemyAI.h"
#include "ButtonSpawnConditions.h"
#include "EngineUtils.h"

// Sets default values
AEnemyGate::AEnemyGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	SpawnerGate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerGate"));
	SpawnerGate->SetupAttachment(MyRoot);

	SpawnerGatePortal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerGatePortal"));
	SpawnerGatePortal->SetupAttachment(MyRoot);

	
	PSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PortalPSC"));
	PSC->SetupAttachment(SpawnerGate);

	bTestingSpawn = false;
}

void AEnemyGate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemyGate, HitPoints);
}

void AEnemyGate::TakeDamages()
{
	if (Role < ROLE_Authority)
	{
		ServerTakeDamages();
		InitiateCheck();

		//InitiateCheck();
	}
	else
	{
		HitPoints--;
		InitiateCheck();

		//InitiateCheck();
		if (HitPoints <= 0)
		{
			//Destroy();
			if (SpawnerGate != nullptr)
			{
				bDeactivated = true;
				ClearSpawnTimer();
				//SpawnerGate->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//SpawnerGate->SetCollisionResponseToAllChannels(ECR_Ignore);
				if (DeactivatedMaterial != nullptr)
					SpawnerGate->SetMaterial(0, DeactivatedMaterial);
				if(PSC != nullptr)
					PSC->DestroyComponent();
				if (SpawnerGatePortal != nullptr)
					SpawnerGatePortal->DestroyComponent();
			}
		}
	}
	// Tried everything - couldn't get this to replicate at all.
	// I'll leave the various attempts in, so you can see the attempt
}

void AEnemyGate::InitiateCheck()
{
	if (HasAuthority())
	{
		MulticastCheckForNewMesh();
	}
	else
	{
		ServerCheckForNewMesh();
	}
}

void AEnemyGate::CheckForNewMesh()
{
	//if (HasAuthority())
	//{
		if (SpawnerGate == nullptr) return;

		// Dead
		if (HitPoints <= 0)
		{
			if (BrokenGateMesh != nullptr)
				SpawnerGate->SetStaticMesh(BrokenGateMesh);
		}
		// HP = 1
		else if (HitPoints < 2)
		{
			if (HitThreeMesh != nullptr)
				SpawnerGate->SetStaticMesh(HitThreeMesh);
		}
		// HP = 2
		else if (HitPoints < 3)
		{
			if (HitTwiceMesh != nullptr)
				SpawnerGate->SetStaticMesh(HitTwiceMesh);
		}
		// HP = 3
		else if (HitPoints < 4)
		{
			if (HitOnceMesh != nullptr)
				SpawnerGate->SetStaticMesh(HitOnceMesh);
		}
	//}
	//else
	//{
		//ServerCheckForNewMesh();
	//}

}

void AEnemyGate::ServerTakeDamages_Implementation()
{
	TakeDamages();
}

bool AEnemyGate::ServerTakeDamages_Validate()
{
	return true;
}

void AEnemyGate::MulticastCheckForNewMesh_Implementation()
{
	CheckForNewMesh();
}

void AEnemyGate::ServerCheckForNewMesh_Implementation()
{
	MulticastCheckForNewMesh();
}

bool AEnemyGate::ServerCheckForNewMesh_Validate()
{
	return true;
}

// Called when the game starts or when spawned
void AEnemyGate::BeginPlay()
{
	Super::BeginPlay();
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		for (TActorIterator<AButtonSpawnConditions> ActorItr(World); ActorItr; ++ActorItr)
		{
			// Fill in Respawn PlayerStarts Array
			SpawnerRef = *ActorItr;
		}
	}

	if (bTestingSpawn)
		BeginSpawning();
}

// Called every frame
void AEnemyGate::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AEnemyGate::BeginSpawning()
{
	
	if (HasAuthority())
	{
		UWorld* const World = GetWorld();
		if (World == nullptr)return;
		World->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemyGate::SpawnEnemy, SpawnFrequency, true);
	}
}

void AEnemyGate::SpawnEnemy()
{
	UWorld* const World = GetWorld();
	if (World == nullptr || bDeactivated) return;
	if (EnemySpawn.Num() >= GateSpawnCap)
	{
		PauseMyTimer();
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEnemyAI* Enemy = World->SpawnActor<AEnemyAI>(EnemyClassToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);
	if (Enemy != nullptr)
	{
		// Can do stuff if you want
		EnemySpawn.Add(Enemy);
		Enemy->OnEnemyDestroyed.AddDynamic(this, &AEnemyGate::RemoveFromGateSpawn);
	}
}

void AEnemyGate::PauseMyTimer()
{
	UWorld* const World = GetWorld();
	if (World == nullptr)return;
	// Pause Timer - so it resumes if an enemy dies
	if (!World->GetTimerManager().IsTimerPaused(SpawnHandle))
	{
		World->GetTimerManager().PauseTimer(SpawnHandle);
	}
}

void AEnemyGate::UnPauseMyTimer()
{
	UWorld* const World = GetWorld();
	if (World == nullptr)return;
	// Pause Timer - so it resumes if an enemy dies
	if (World->GetTimerManager().IsTimerPaused(SpawnHandle))
	{
		World->GetTimerManager().UnPauseTimer(SpawnHandle);
	}
}

void AEnemyGate::ClearSpawnTimer()
{
	UWorld* const World = GetWorld();
	if (World == nullptr)return;
	World->GetTimerManager().ClearTimer(SpawnHandle);
}

void AEnemyGate::RemoveFromGateSpawn(AEnemyAI* _Enemy)
{
	if (_Enemy != nullptr && EnemySpawn.Num() > 0)
	{
		if (bWantsToSpawnAButton)
		{
			SpawnRequirement--;
			UE_LOG(LogTemp, Warning, TEXT("Spawn Req: %d"), SpawnRequirement);
			if (SpawnRequirement <= 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Target Met for Spawn"));

				if (SpawnerRef != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("SpawnedButton"));

					SpawnerRef->SpawnButton();
				}
				bWantsToSpawnAButton = false;
			}
		}
		
		EnemySpawn.Remove(_Enemy);
	}
}
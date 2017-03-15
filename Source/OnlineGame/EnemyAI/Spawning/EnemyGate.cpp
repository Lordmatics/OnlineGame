// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "EnemyAI/Spawning/EnemyGate.h"
#include "EnemyAI/EnemyAI.h"

// Sets default values
AEnemyGate::AEnemyGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	SpawnerGate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerGate"));
	SpawnerGate->SetupAttachment(MyRoot);

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
	}
	else
	{
		HitPoints--;
		if (HitPoints <= 0)
		{
			Destroy();
		}
	}
}

void AEnemyGate::ServerTakeDamages_Implementation()
{
	TakeDamages();
}

bool AEnemyGate::ServerTakeDamages_Validate()
{
	return true;
}

// Called when the game starts or when spawned
void AEnemyGate::BeginPlay()
{
	Super::BeginPlay();
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
	if (World == nullptr)return;
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
		EnemySpawn.Remove(_Enemy);
	}
}
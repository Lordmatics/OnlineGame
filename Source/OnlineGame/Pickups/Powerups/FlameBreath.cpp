// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Pickups/Powerups/FlameBreath.h"
#include "OnlineGameCharacter.h"
#include "EnemyAI/EnemyAI.h"

AFlameBreath::AFlameBreath()
{
	bReplicates = true;

	FlameTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("FlameTrigger"));
	FlameTrigger->bGenerateOverlapEvents = true;
	FlameTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFlameBreath::OnFlameEnter);
	FlameTrigger->OnComponentEndOverlap.AddDynamic(this, &AFlameBreath::OnFlameExit);
	FlameTrigger->SetupAttachment(MyRoot);
}

void AFlameBreath::BeginPlay()
{
	Super::BeginPlay();
}

void AFlameBreath::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlameBreath, FlameBreath);
	DOREPLIFETIME(AFlameBreath, FlameBreathParticleSystem);
}

void AFlameBreath::TriggerEnter_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnlineGameCharacter* OnlineCharacter = Cast<AOnlineGameCharacter>(OtherActor);
	if (OnlineCharacter != nullptr)
	{
		// Add a Powerup to players inventory or something
		OnlineCharacter->ObtainPower(this);

		// Sound - Effect - Destroy
		Super::TriggerEnter_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

		// Move Trigger
		UE_LOG(LogTemp, Warning, TEXT("FlameTrigger Supposed To Snap"));
		FlameTrigger->AttachToComponent(PowerupOwner, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachPoint);
	}
}

void AFlameBreath::OnFlameEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (PowerupOwnerClass != nullptr && OtherActor != nullptr)
	//{
		//PowerupOwnerClass->DealDamage(OtherActor);
		AEnemyAI* Enemy = Cast<AEnemyAI>(OtherActor);
		if (Enemy != nullptr)
		{
			EnemyList.Add(Enemy);
			UE_LOG(LogTemp, Warning, TEXT("EnemyAdded: ArrayLength: %d"), EnemyList.Num());
			//Enemy->TakeDamageOverTime(FlameDPS);
		}
	//}
}

void AFlameBreath::OnFlameExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEnemyAI* Enemy = Cast<AEnemyAI>(OtherActor);
	if (Enemy != nullptr)
	{
		EnemyList.Remove(Enemy);
		UE_LOG(LogTemp, Warning, TEXT("EnemyRemoved: ArrayLength: %d"), EnemyList.Num());
		Enemy->ClearDOT();
	}
}

void AFlameBreath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Debug code - Custom Timer - Seems actor dead so couldnt run functions
	//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("RunningTime: %f"), RunningTime));

	//if (bFlameOn)
	//{
	//	RunningTime += DeltaTime;
	//	if (RunningTime >= TimeTillFlamesBurnOut)
	//	{
	//		bFlameOn = false;
	//		RunningTime = 0.0f;
	//		if (FlameBreath != nullptr)
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("KillFlames"));
	//			FlameBreath->DeactivateSystem();
	//			FlameBreath = nullptr;
	//		}
	//	}
	//}
}

/** Override this specific to each powerup - might be redundant not sure yet*/
void AFlameBreath::UsePower()
{
	UE_LOG(LogTemp, Warning, TEXT("FlameBreath: UsePower Activated"));
	if (!bFlameOn)
	{
		MulticastRPCFunction_SpawnFlameBreath();
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			//KillFlames();
			bFlameOn = true;
			FTimerHandle TempHandle;
			World->GetTimerManager().SetTimer(TempHandle, this, &AFlameBreath::KillFlames, TimeTillFlamesBurnOut, false);
			//UE_LOG(LogTemp, Warning, TEXT("HELLO TIMER PLS: %f"), TimeTillFlamesBurnOut);
			//UE_LOG(LogTemp, Warning, TEXT("HELLO TIMER PLS: %s"), this == nullptr ? TEXT("YEA") : TEXT("NAH"));
			if (EnemyList.Num() > 0)
			{
				for (AEnemyAI* Enemy : EnemyList)
				{
					if (Enemy != nullptr)
					{
						// Perhaps change to component burning thing
						Enemy->TakeDamageOverTime(FlameDPS);
					}
				}
			}
		}
		Super::UsePower();
		
	}
}

void AFlameBreath::MulticastRPCFunction_SpawnFlameBreath_Implementation()
{
	UWorld* const World = GetWorld();
	if ((World == nullptr || FlameBreathParticleSystem == nullptr) && FlameBreath != nullptr || PowerupOwner == nullptr) return;// false;
	FlameBreath = UGameplayStatics::SpawnEmitterAttached(FlameBreathParticleSystem, PowerupOwner, AttachPoint);
	if (FlameBreath != nullptr)
	{
		FlameBreath->SetIsReplicated(true);
		//return true;
	}
	//return false;
}

void AFlameBreath::UseCharge()
{
	if (GetPowerCharges() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerUpUsedFully - Destroyed"));
		Destroy();
	}
	Super::UseCharge();

}

bool AFlameBreath::SpawnFlameBreath()
{
	//if (HasAuthority())
	//{
	//	// Spawn on Server and Replicate to all clients
	//	// Thinking OnRepNotify for deactivate but need halp
	//}
	//else
	//{

	//}
	UWorld* const World = GetWorld();
	if ((World == nullptr || FlameBreathParticleSystem == nullptr) && FlameBreath != nullptr || PowerupOwner == nullptr) return false;
	FlameBreath = UGameplayStatics::SpawnEmitterAttached(FlameBreathParticleSystem, PowerupOwner, AttachPoint);
	if (FlameBreath != nullptr && FlameTrigger != nullptr)
	{

		FlameBreath->SetIsReplicated(true);
		return true;
	}
	return false;
	//World->GetTimerManager().SetTimer(TempHandle, this, &AFlameBreath::KillFlames, TimeTillFlamesBurnOut, false);
}

void AFlameBreath::MulticastRPCFunction_KillFlames_Implementation()
{
	if (FlameBreath != nullptr && FlameTrigger != nullptr)
	{
		FlameBreath->DeactivateSystem();
		FlameBreath = nullptr;
		FlameTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bFlameOn = false;
	}
}

void AFlameBreath::KillFlames()
{
	MulticastRPCFunction_KillFlames();
	//if (FlameBreath != nullptr)
	//{
	//	FlameBreath->DeactivateSystem();
	//	FlameBreath = nullptr;
	//	bFlameOn = false;
	//}
}

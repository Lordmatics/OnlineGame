// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Pickups/Powerups/FlameBreath.h"


AFlameBreath::AFlameBreath()
{
	bReplicates = true;
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
	if (FlameBreath != nullptr)
	{
		FlameBreath->SetIsReplicated(true);
		return true;
	}
	return false;
	//World->GetTimerManager().SetTimer(TempHandle, this, &AFlameBreath::KillFlames, TimeTillFlamesBurnOut, false);
}

void AFlameBreath::MulticastRPCFunction_KillFlames_Implementation()
{
	if (FlameBreath != nullptr)
	{
		FlameBreath->DeactivateSystem();
		FlameBreath = nullptr;
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

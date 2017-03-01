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

void AFlameBreath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** Override this specific to each powerup - might be redundant not sure yet*/
void AFlameBreath::UsePower()
{
	UE_LOG(LogTemp, Warning, TEXT("FlameBreath: UsePower Activated"));
	if (SpawnFlameBreath())
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			//KillFlames();
			PLEASE();
			UE_LOG(LogTemp, Warning, TEXT("HELLO TIMER PLS: %f"), TimeTillFlamesBurnOut);
			UE_LOG(LogTemp, Warning, TEXT("HELLO TIMER PLS: %s"), this == nullptr ? TEXT("YEA") : TEXT("NAH"));

		}
		Super::UsePower();
	}

}

bool AFlameBreath::SpawnFlameBreath()
{
	UWorld* const World = GetWorld();
	if ((World == nullptr || FlameBreathParticleSystem == nullptr) && FlameBreath != nullptr || PowerupOwner == nullptr) return false;
	FlameBreath = UGameplayStatics::SpawnEmitterAttached(FlameBreathParticleSystem, PowerupOwner, AttachPoint);
	if (FlameBreath != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Flames On"));
		return true;
	}
	return false;
	//World->GetTimerManager().SetTimer(TempHandle, this, &AFlameBreath::KillFlames, TimeTillFlamesBurnOut, false);
}

void AFlameBreath::KillFlames()
{
	UE_LOG(LogTemp, Warning, TEXT("KillFlames NULL"));

	if (FlameBreath != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("KillFlames"));
		FlameBreath->DeactivateSystem();
		FlameBreath = nullptr;
	}
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().ClearTimer(TempHandle);
	}
}

void AFlameBreath::PLEASE()
{
	UE_LOG(LogTemp, Warning, TEXT("PLEASE RAN"));
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	World->GetTimerManager().SetTimer(TempHandle, this, &AFlameBreath::KillFlames, 1.0f, false, 1.0f);

}
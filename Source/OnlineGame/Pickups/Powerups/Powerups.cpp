// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Pickups/Powerups/Powerups.h"
#include "OnlineGameCharacter.h"

APowerups::APowerups()
{
	bReplicates = true;
}

void APowerups::BeginPlay()
{
	Super::BeginPlay();
}

void APowerups::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** Call this Super::Function at the end of the overriden logic - since this handles the logic, that every pickup will do*/
void APowerups::TriggerEnter_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnlineGameCharacter* OnlineCharacter = Cast<AOnlineGameCharacter>(OtherActor);
	if (OnlineCharacter != nullptr)
	{
		// Add a Powerup to players inventory or something
		OnlineCharacter->ObtainPower(this);
		UE_LOG(LogTemp, Warning, TEXT("BasePowerUps: PowerUp Obtained"));
		// Sound - Effect - Destroy
		Super::TriggerEnter_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

/** Override this specific to each powerup - might be redundant not sure yet*/
void APowerups::UsePower()
{
	UE_LOG(LogTemp, Warning, TEXT("BasePowerUps: UsePower Activated"));
	UseCharge();
}

void APowerups::UseCharge()
{
	if (PowerCharges == 0) return;
	PowerCharges--;
	UE_LOG(LogTemp, Warning, TEXT("Charges: %d"), PowerCharges);
	PowerCharges = FMath::Clamp(PowerCharges, 0, MaxCharges);
}
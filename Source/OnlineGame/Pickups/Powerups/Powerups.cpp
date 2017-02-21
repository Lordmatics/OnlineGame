// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Pickups/Powerups/Powerups.h"
#include "OnlineGameCharacter.h"

APowerups::APowerups()
{

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
		PowerUpObtained();

		// Sound - Effect - Destroy
		Super::TriggerEnter_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

/** Override this specific to each powerup - might be redundant not sure yet*/
void APowerups::PowerUpObtained()
{

}
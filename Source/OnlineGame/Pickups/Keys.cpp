// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Pickups/Keys.h"
#include "OnlineGameCharacter.h"

AKeys::AKeys()
{
	bReplicates = true;
}

void AKeys::BeginPlay()
{
	Super::BeginPlay();
}

void AKeys::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** Call this Super::Function at the end of the overriden logic - since this handles the logic, that every pickup will do*/
void AKeys::TriggerEnter_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnlineGameCharacter* OnlineCharacter = Cast<AOnlineGameCharacter>(OtherActor);
	if (OnlineCharacter != nullptr)
	{
		// Add a key to players inventory or something
		//

		// Sound - Effect - Destroy
		Super::TriggerEnter_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}

}
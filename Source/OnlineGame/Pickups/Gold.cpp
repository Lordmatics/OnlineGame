// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Pickups/Gold.h"
#include "OnlineGameCharacter.h"

AGold::AGold()
{
	bReplicates = true;
}

void AGold::BeginPlay()
{
	Super::BeginPlay();
}

void AGold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** Call this Super::Function at the end of the overriden logic - since this handles the logic, that every pickup will do*/
void AGold::TriggerEnter_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnlineGameCharacter* OnlineCharacter = Cast<AOnlineGameCharacter>(OtherActor);
	if (OnlineCharacter != nullptr)
	{
		// Add Gold to players inventory or something
		//
		OnlineCharacter->ObtainGold(GoldValue);
		UE_LOG(LogTemp, Warning, TEXT("%s has %f Gold"), *OnlineCharacter->GetName(), OnlineCharacter->GetGoldCount());
		// Sound - Effect - Destroy
		Super::TriggerEnter_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}
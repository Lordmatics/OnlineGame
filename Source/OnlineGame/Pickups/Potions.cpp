// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Potions.h"
#include "OnlineGameCharacter.h"


void APotions::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

APotions::APotions()
{
	bReplicates = true;
}

void APotions::BeginPlay()
{

}

void APotions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APotions::TriggerEnter_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnlineGameCharacter* OnlineCharacter = Cast<AOnlineGameCharacter>(OtherActor);
	if (OnlineCharacter != nullptr)
	{
		// Add a key to players inventory or something
		//
		OnlineCharacter->GainPotion();
		UE_LOG(LogTemp, Warning, TEXT("%s has %d keys"), *OnlineCharacter->GetName(), OnlineCharacter->GetKeyCount());
		// Sound - Effect - Destroy
		Super::TriggerEnter_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}
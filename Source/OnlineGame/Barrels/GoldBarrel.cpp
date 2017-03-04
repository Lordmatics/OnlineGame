// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "GoldBarrel.h"
#include "Pickups/Pickups.h"


// Sets default values
AGoldBarrel::AGoldBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGoldBarrel::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGoldBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoldBarrel::OnFractureOverride(const FVector& HitPoint, const FVector& HitDirection)
{
	DropItem();
	Super::OnFractureOverride(HitPoint, HitDirection);
}

void AGoldBarrel::DropItem()
{
	UWorld* const World = GetWorld();
	if (World == nullptr || BarrelDrop == nullptr) return;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	World->SpawnActor<APickups>(BarrelDrop, RadialForce->GetComponentLocation(), GetActorRotation(), SpawnParams);
}
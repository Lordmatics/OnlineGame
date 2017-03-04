// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "ExplosiveBarrel.h"

// Sets default values for this actor's properties
AExplosiveBarrel::AExplosiveBarrel()
{

}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AExplosiveBarrel::OnFractureOverride(const FVector& HitPoint, const FVector& HitDirection)
{
	MulticastRPCFunction_CreateExplosion();
	Super::OnFractureOverride(HitPoint, HitDirection);
}

void AExplosiveBarrel::MulticastRPCFunction_CreateExplosion_Implementation()
{
	if (ExplosionEmitter == nullptr) return;
	UParticleSystemComponent* ExplosionPS = UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEmitter, GetActorLocation());
	if (ExplosionPS != nullptr)
	{
		ExplosionPS->SetIsReplicated(true);
		ExplosionPS->SetWorldScale3D(FVector(ExplosionSize));
	}
}


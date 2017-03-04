// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Barrels/Barrels.h"
#include "ExplosiveBarrel.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API AExplosiveBarrel : public ABarrels
{
	GENERATED_BODY()
	
protected:
	virtual void OnFractureOverride(const FVector& HitPoint, const FVector& HitDirection);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRPCFunction_CreateExplosion();

	UPROPERTY(EditAnywhere, Category = "C++ Explosive Barrel")
		UParticleSystem* ExplosionEmitter;

	UPROPERTY(EditAnywhere, Category = "C++ Explosive Barrel")
		float ExplosionSize = 3.0f;
public:
	// Sets default values for this actor's properties
	AExplosiveBarrel();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
	
};

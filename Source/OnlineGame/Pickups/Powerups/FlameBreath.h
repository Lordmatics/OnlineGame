// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickups/Powerups/Powerups.h"
#include "FlameBreath.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API AFlameBreath : public APowerups
{
	GENERATED_BODY()
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(NetMulticast, Reliable)
		void MulticastRPCFunction_SpawnFlameBreath();

	bool SpawnFlameBreath();
	
	UPROPERTY(EditAnywhere, Category = "C++ Power Ups", Replicated)
		UParticleSystemComponent* FlameBreath;

	UPROPERTY(EditAnywhere, Category = "C++ Power Ups", Replicated)
		UParticleSystem* FlameBreathParticleSystem;

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRPCFunction_KillFlames();

	UFUNCTION()
		void KillFlames();

	UPROPERTY(EditAnywhere, Category = "C++ Power Ups")
		float TimeTillFlamesBurnOut = 1.0f;

	UPROPERTY(EditAnywhere, Category = "C++ Power Ups")
		uint32 bFlameOn : 1;

public:

	// Sets default values for this actor's properties
	AFlameBreath();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Function to actually do something - ie shoot fire*/
	virtual void UsePower() override;
	
};

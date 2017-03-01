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
	
private:
	bool SpawnFlameBreath();
	
	UPROPERTY(EditAnywhere, Category = "C++ Power Ups")
		UParticleSystemComponent* FlameBreath;

	UPROPERTY(EditAnywhere, Category = "C++ Power Ups")
		UParticleSystem* FlameBreathParticleSystem;

	UFUNCTION()
		void KillFlames();

	UFUNCTION()
		void PLEASE();

	UPROPERTY(EditAnywhere, Category = "C++ Power Ups")
		float TimeTillFlamesBurnOut = 1.0f;

	FTimerHandle TempHandle;

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

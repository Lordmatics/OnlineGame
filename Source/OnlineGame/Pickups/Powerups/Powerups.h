// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickups/Pickups.h"
#include "Powerups.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API APowerups : public APickups
{
	GENERATED_BODY()
	
private:

protected:

	/** Overridable function to do something when overlapped*/
	virtual void TriggerEnter_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	//virtual void PowerUpObtained();
	UPROPERTY(EditAnywhere, Category = "C++ Power Ups")
		int PowerCharges = 5;

	UPROPERTY(EditAnywhere, Category = "C++ Power Ups")
		int MaxCharges = 30;

	/** Function to deduct a use from total amount*/
	void UseCharge();
public:
	
	// Sets default values for this actor's properties
	APowerups();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Function to actually do something - ie shoot fire*/
	virtual void UsePower();

	FORCEINLINE int GetPowerCharges() const { return PowerCharges; }


};

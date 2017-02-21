// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickups/Pickups.h"
#include "Keys.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API AKeys : public APickups
{
	GENERATED_BODY()
	
private:

protected:

	/** Overridable function to do something when overlapped*/
	virtual void TriggerEnter_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
public:
	
	// Sets default values for this actor's properties
	AKeys();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
};

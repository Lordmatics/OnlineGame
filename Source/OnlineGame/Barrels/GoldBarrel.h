// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Barrels/Barrels.h"
#include "GoldBarrel.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API AGoldBarrel : public ABarrels
{
	GENERATED_BODY()
private:

	void DropItem();

protected:
	virtual void OnFractureOverride(const FVector& HitPoint, const FVector& HitDirection) override;

	UPROPERTY(EditAnywhere, Category = "C++ Barrel")
		TSubclassOf<class APickups> BarrelDrop;
public:

	AGoldBarrel();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	
	
};

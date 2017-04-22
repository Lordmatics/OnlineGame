// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ButtonSpawnConditions.generated.h"

UCLASS()
class ONLINEGAME_API AButtonSpawnConditions : public AActor
{
	GENERATED_BODY()
private:

	UPROPERTY(EditAnywhere, Category = "C++ Button Class")
		TSubclassOf<class AButtonSwitch> ButtonClass;

	UPROPERTY(EditAnywhere, Category = "C++ Button Class")
		TArray<AActor* > SpawnLocations;

	UPROPERTY(EditAnywhere, Category = "C++ Button Class")
		TArray<class AKeyGate* > ButtonGates;

	int Counter = 0;
private:

public:	
	// Sets default values for this actor's properties
	AButtonSpawnConditions();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SpawnButton();


	
	
};

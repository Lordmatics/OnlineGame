// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Lantern.generated.h"

UCLASS()
class ONLINEGAME_API ALantern : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Component")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Component")
		UStaticMeshComponent* MyLantern;

	/** Looped Flames or something*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Component")
		UParticleSystemComponent* ParticleSystemComponent;
	
	// Can Edit Material in Code
	UMaterialInstanceDynamic* DynMaterial;

	// Name of Parameter I want to manipulate
	UPROPERTY(EditAnywhere, Category = "Dynamic Material")
		FName DynMaterialParam = FName("Emissive");
public:	
	// Sets default values for this actor's properties
	ALantern();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};

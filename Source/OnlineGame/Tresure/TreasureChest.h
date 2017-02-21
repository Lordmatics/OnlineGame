// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TreasureChest.generated.h"

UCLASS()
class ONLINEGAME_API ATreasureChest : public AActor
{
	GENERATED_BODY()
private:
	/** Root Component*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Treasure Chest")
		USceneComponent* MyRoot;

	// Solid Mesh until trigger zone activated
	/** Skeletal Treasure chest for animation*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Treasure Chest")
		USkeletalMeshComponent* MyTreasureChest;

	/** Chest to swap with at the end of the montage*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Treasure Chest")
		USkeletalMeshComponent* MyTreasureChestEnd;

	/** Set this collision to Pickup - so it only overlaps pawn*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Treasure Chest")
		UBoxComponent* MyTriggerZone;

	/** Bool to determine whether chest can be openned*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Treasure Chest")
		uint32 bTreasureChestIsOpen : 1;

	/** Type of gold that will reside inside the treasure*/
	UPROPERTY(EditAnywhere, Category = "C++ Treasure Chest")
		TSubclassOf<class APickups> GoldClass;

	/** Gold set to always spawn - can offset its location from this actor with this*/
	UPROPERTY(EditAnywhere, Category = "C++ Treasure Chest")
		FVector SpawnOffset = FVector();

	/** Animation Play speed*/
	UPROPERTY(EditAnywhere, Category = "C++ Treasure Chest")
		float PlaySpeed = 1.0f;

	/** Animation to open up chest*/
	UPROPERTY(EditAnywhere, Category = "C++ Treasure Chest")
		UAnimMontage* OpenAnim;
private:
	UFUNCTION()
		void OnTreasureChestOpenned();

	void SpawnGold();
public:	
	// Sets default values for this actor's properties
	ATreasureChest();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Function to disable BoxZone and SM Collision after anim duration*/
	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
};

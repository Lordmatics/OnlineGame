// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "KeyGate.generated.h"

UCLASS()
class ONLINEGAME_API AKeyGate : public AActor
{
	GENERATED_BODY()
	
private:
	/** Root Component*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Treasure Chest")
		USceneComponent* MyRoot;

	// Solid Mesh until trigger zone activated
	/** Skeletal Gate for animation*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Treasure Chest")
		USkeletalMeshComponent* MyGate;

	/** Gate to swap with at the end of the montage*/
	// HAD COLLISION WIERDNESS WITH PHYSICS ASSET - USING STATIC MESH FOR THE END BIT
	//UPROPERTY(VisibleDefaultsOnly, Category = "C++ Treasure Chest")
	//	USkeletalMeshComponent* MyGateEnd;

	/** Gate to swap with at the end of the montage*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Treasure Chest")
		UStaticMeshComponent* MyGateEndSM;

	/** Set this collision to Pickup - so it only overlaps pawn*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Treasure Chest")
		UBoxComponent* MyTriggerZone;

	/** Bool to determine whether Gate can be openned*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Treasure Chest")
		uint32 bGateOpen : 1;

	/** Gold set to always spawn - can offset its location from this actor with this*/
	UPROPERTY(EditAnywhere, Category = "C++ Treasure Chest")
		FVector SpawnOffset = FVector();

	/** Animation Play speed*/
	UPROPERTY(EditAnywhere, Category = "C++ Treasure Chest")
		float PlaySpeed = 1.0f;

	/** In case animation time is wonky*/
	UPROPERTY(EditAnywhere, Category = "C++ Treasure Chest")
		float AnimDurationOffset = -0.25f;

	/** Animation to open up chest*/
	UPROPERTY(EditAnywhere, Category = "C++ Treasure Chest")
		UAnimMontage* OpenAnim;

	UPROPERTY(EditAnywhere, Category = "C++ Event Gate")
		uint32 bButtonNotKey : 1;

	UPROPERTY(EditAnywhere, Category = "C++ Event Gate")
		class AButtonSwitch* Button;
private:
	
	bool OpenGate();

	UFUNCTION()
		void OnGateOpen();
public:	
	// Sets default values for this actor's properties
	AKeyGate();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Function to disable BoxZone and SM Collision after anim duration*/
	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
	FORCEINLINE void SetButtonRef(class AButtonSwitch* _Ref) { Button = _Ref; }
};

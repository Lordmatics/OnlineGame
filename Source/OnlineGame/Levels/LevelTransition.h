// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LevelTransition.generated.h"

UCLASS()
class ONLINEGAME_API ALevelTransition : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ LevelTransition")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ LevelTransition")
		UStaticMeshComponent* MyStaticMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ LevelTransition")
		UBoxComponent* MyTrigger;

	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
		void OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = "C++ LevelTransition")
		int Counter = 0;

	UPROPERTY(VisibleAnywhere, Category = "C++ LevelTransition")
		class AOnlineGameGameMode* MyGameMode;
public:	
	// Sets default values for this actor's properties
	ALevelTransition();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};

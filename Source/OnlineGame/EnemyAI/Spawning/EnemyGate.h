// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EnemyGate.generated.h"

UCLASS()
class ONLINEGAME_API AEnemyGate : public AActor
{
	GENERATED_BODY()
	
private:
	
	UPROPERTY(EditAnywhere, Category = "C++")
		uint32 bTestingSpawn : 1;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++")
		UStaticMeshComponent* SpawnerGate;

	UPROPERTY(EditAnywhere, Category = "C++")
		TSubclassOf<class AEnemyAI> EnemyClassToSpawn;

	UPROPERTY(EditAnywhere, Category = "C++")
		float SpawnFrequency = 3.0f;

	UPROPERTY(EditAnywhere, Category = "C++")
		int GateSpawnCap = 10;

	void BeginSpawning();

	UFUNCTION()
		void SpawnEnemy();

	FTimerHandle SpawnHandle;

	UFUNCTION()
		void ClearSpawnTimer();

	UPROPERTY(VisibleAnywhere, Category = "C++ AI")
		TArray<class AEnemyAI*> EnemySpawn;

	UFUNCTION()
		void RemoveFromGateSpawn(class AEnemyAI* _Enemy);

	void PauseMyTimer();

	void UnPauseMyTimer();
public:	
	// Sets default values for this actor's properties
	AEnemyGate();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};

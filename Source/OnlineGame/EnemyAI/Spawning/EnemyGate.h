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

	UPROPERTY(EditAnywhere, Category = "C++", Replicated)
		int HitPoints = 3;

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

	//UFUNCTION()
	//	void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerTakeDamages();
	virtual void ServerTakeDamages_Implementation();
	virtual bool ServerTakeDamages_Validate();

public:	
	// Sets default values for this actor's properties
	AEnemyGate();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Function to reduce hitpoints- calls private server handling functions
	UFUNCTION()
		void TakeDamages();
	
	
};

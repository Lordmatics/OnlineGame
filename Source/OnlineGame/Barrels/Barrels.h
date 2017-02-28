// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Barrels.generated.h"

UCLASS()
class ONLINEGAME_API ABarrels : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Barrel", meta = (AllowPrivateAccess = "true"))
		USceneComponent* MyRoot;

	UPROPERTY(EditAnywhere, Category = "C++ Barrel")
		uint32 bKillStaticMesh : 1;

	UFUNCTION()
		void OnComponentFracture(const FVector& HitPoint, const FVector& HitDirection);

	UFUNCTION()
		void DestroyThis();

	UFUNCTION()
		void DisableCollision();
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Barrel", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BarrelSM;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Barrel", meta = (AllowPrivateAccess = "true"))
		class UDestructibleComponent* BarrelDM;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Barrel", meta = (AllowPrivateAccess = "true"))
		class URadialForceComponent* RadialForce;

	virtual void OnFractureOverride(const FVector& HitPoint, const FVector& HitDirection);

public:	
	// Sets default values for this actor's properties
	ABarrels();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Fracture();

	FORCEINLINE bool IsStaticDestroyable() const { return bKillStaticMesh; }
	
};

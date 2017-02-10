// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "RaycastComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONLINEGAME_API URaycastComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	//UPROPERTY(EditAnywhere, Category = "Raycast")
	//	float Raylength = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Raycast")
		UMaterialInterface* BulletHoleDecal;

	UPROPERTY(EditAnywhere, Category = "Raycast")
		FVector DecalSize = FVector(3.5f, 7.0f, 7.0f);

	UPROPERTY(EditAnywhere, Category = "Raycast")
		float BulletDecalLifetime = 20.0f;

	void CreateBulletHole(FHitResult Object);
public:
	// Sets default values for this component's properties
	URaycastComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FHitResult RaycastTP(USkeletalMeshComponent* Mesh, FVector ForwardVector, float CustomLength = 5000.0f);

	FHitResult RaycastFP(UCameraComponent* FirstPersonCamera, float CustomLength = 5000.0f);

	FHitResult RaycastLaser(UParticleSystemComponent* FirstPersonCamera, float CustomLength = 5000.0f);

	//FORCEINLINE float GetRayLength() const { return Raylength; }

	FORCEINLINE UMaterialInterface* GetBulletHoleDecal() const { return BulletHoleDecal; }

};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickups.generated.h"

UCLASS()
class ONLINEGAME_API APickups : public AActor
{
	GENERATED_BODY()
private:

	/** The Function that gets called on all versions of this class - which then triggers the overridable implementation function*/
	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlayPickUpSound(const FVector& Location);

	void PlayPickUpEffect(const FVector& Location);

	UFUNCTION()
		void HandleOverlap();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerHandleOverlap();
protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Default Root Component for Object*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Base Class")
		USceneComponent* MyRoot;

	/** Mesh used to represent object - has collision*/
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Base Class")
		UStaticMeshComponent* MyStaticMesh;

	/** Audio feedback - for when you pick up an item - could replicate this sound but might not*/
	UPROPERTY(EditAnywhere, Category = "C++ Base Class")
		USoundBase* PickUpSound;

	/** Visual feedback - for when you pick up an item - should replicate probably*/
	UPROPERTY(EditAnywhere, Category = "C++ Base Class", Replicated)
		UParticleSystem* PickUpEffect;

	/** Volume of sound - so it doesnt bite your ears off*/
	UPROPERTY(EditAnywhere, Category = "C++ Base Class")
		float SoundVolume = 0.25f;

	/** Overridable function to do something when overlapped*/
	virtual void TriggerEnter_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Sets default values for this actor's properties
	APickups();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ButtonSwitch.generated.h"

UCLASS()
class ONLINEGAME_API AButtonSwitch : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ ButtonSwitch")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ ButtonSwitch")
		UStaticMeshComponent* MyStaticMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ ButtonSwitch")
		UBoxComponent* MyTrigger;

	UPROPERTY(VisibleDefaultsOnly, Category = "C++ ButtonSwitch")
		UParticleSystemComponent* PSC;

	UPROPERTY(EditAnywhere, Category = "C++ ButtonSwitch")
		UParticleSystem* ActivePS;

	UPROPERTY(EditAnywhere, Category = "C++ ButtonSwitch")
		UParticleSystem* InactivePS;

	UPROPERTY(VisibleAnywhere, Category = "C++ ButtonSwitch", ReplicatedUsing = OnRep_Pressed)
		bool bPressed;

	UFUNCTION()
		virtual void OnRep_Pressed();

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerSetPressed(bool _Pressed);
	virtual void ServerSetPressed_Implementation(bool _Pressed);
	virtual bool ServerSetPressed_Validate(bool _Pressed);
private:
	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ActivatePS();

	void DeactivatePS();
public:	
	// Sets default values for this actor's properties
	AButtonSwitch();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE bool GetPressed() const { return bPressed; }
	FORCEINLINE void SetPressed(bool _Pressed) { bPressed = _Pressed; }
	
};

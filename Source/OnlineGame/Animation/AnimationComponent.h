// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "AnimationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONLINEGAME_API UAnimationComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "C++ Animations")
		UAnimMontage* AttackAnim;

public:	
	// Sets default values for this component's properties
	UAnimationComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	// Forceinlines
	FORCEINLINE UAnimMontage* GetAttackAnimMontage() const { return AttackAnim; }
};

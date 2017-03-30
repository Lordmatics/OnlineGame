// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "FlickerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONLINEGAME_API UFlickerComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UFUNCTION()
		void Flicker();

	UFUNCTION()
		void ToggleDelay();

	/** Change this via the lights properties*/
	UPROPERTY(VisibleAnywhere, Category = "C++ Light")
		float InitialIntensity = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "C++ Light")
		float ToggleWait = 1.0f;

	UPROPERTY(EditAnywhere, Category = "C++ Light")
		float ToggleRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "C++ Light")
		uint32 bToggleVisiblity : 1;
public:	
	// Sets default values for this component's properties
	UFlickerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};

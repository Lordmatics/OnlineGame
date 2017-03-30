// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "FlickerComponent.h"
#include "Classes/Engine/Light.h"

// Sets default values for this component's properties
UFlickerComponent::UFlickerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UFlickerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (!bToggleVisiblity) return;

	ALight* LightSource = Cast<ALight>(GetOwner());
	if (LightSource != nullptr)
	{
		InitialIntensity = LightSource->GetLightComponent()->Intensity;
	}

	FTimerHandle TempHandle;
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	World->GetTimerManager().SetTimer(TempHandle, this, &UFlickerComponent::ToggleDelay, ToggleWait, false);
	
}


// Called every frame
void UFlickerComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UFlickerComponent::ToggleDelay()
{
	FTimerHandle TempHandle;
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	World->GetTimerManager().SetTimer(TempHandle, this, &UFlickerComponent::Flicker, ToggleRate, true);
}

void UFlickerComponent::Flicker()
{
	ALight* LightSource = Cast<ALight>(GetOwner());
	if (LightSource != nullptr)
	{
		ULightComponent* LightComp = LightSource->GetLightComponent();
		if (LightComp == nullptr) return;
		// Alternate between starting intensity and no intenisty
		LightComp->Intensity == 0 ? LightComp->SetIntensity(InitialIntensity) : LightComp->SetIntensity(0.0f);
	}
}
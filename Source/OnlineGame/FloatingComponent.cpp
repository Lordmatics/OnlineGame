// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "FloatingComponent.h"


// Sets default values for this component's properties
UFloatingComponent::UFloatingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFloatingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFloatingComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	Float(DeltaTime);
	// ...
}

void UFloatingComponent::Float(float DeltaTime)
{
	if (!bCanFloat)return;
	FVector NewLocation = GetOwner()->GetActorLocation();
	float DeltaHeight = (FMath::Sin(CurrentTime + DeltaTime) - FMath::Sin(CurrentTime));
	NewLocation.Z += DeltaHeight * Height;  //Scale our height by a factor of 20
	CurrentTime += DeltaTime * Speed;
	GetOwner()->SetActorLocation(NewLocation);
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Barrels.h"
#include "Components/DestructibleComponent.h"
#include "Classes/PhysicsEngine/RadialForceComponent.h"

// Sets default values
ABarrels::ABarrels()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = MyRoot;

	BarrelSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelSM"));
	BarrelSM->SetupAttachment(MyRoot);

	BarrelDM = CreateDefaultSubobject<UDestructibleComponent>(TEXT("BarrelDM"));
	BarrelDM->OnComponentFracture.AddDynamic(this, &ABarrels::OnComponentFracture);
	BarrelDM->bEnableHardSleeping = true;
	BarrelDM->LargeChunkThreshold = 5.0f;
	BarrelDM->SetupAttachment(MyRoot);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(BarrelDM);
}

// Called when the game starts or when spawned
void ABarrels::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABarrels::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABarrels::Fracture()
{
	if (BarrelDM != nullptr && BarrelSM != nullptr)
	{
		if (RadialForce != nullptr)
		{
			RadialForce->FireImpulse();
		}
	}
}

void ABarrels::OnFractureOverride(const FVector& HitPoint, const FVector& HitDirection)
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		if (BarrelSM != nullptr && bKillStaticMesh)
		{
			BarrelSM->DestroyComponent();
		}
		DisableCollision();
		//FTimerHandle CollisionTimer;
		//World->GetTimerManager().SetTimer(CollisionTimer, this, &ABarrels::DisableCollision, 1.5f, false);

		FTimerHandle DestroyTimer;
		World->GetTimerManager().SetTimer(DestroyTimer, this, &ABarrels::DestroyThis, 5.0f, false);

	}
}

void ABarrels::OnComponentFracture(const FVector& HitPoint, const FVector& HitDirection)
{
	OnFractureOverride(HitPoint, HitDirection);
}

void ABarrels::DisableCollision()
{
	if (BarrelDM != nullptr)
	{
		BarrelDM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABarrels::DestroyThis()
{
	BarrelDM->DestroyComponent();
}
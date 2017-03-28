// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Environment/Lantern.h"


// Sets default values
ALantern::ALantern()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	MyLantern = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyLantern"));
	MyLantern->SetupAttachment(MyRoot);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleSystemComponent->SetupAttachment(MyLantern);

	DynMaterial = MyLantern->CreateDynamicMaterialInstance(0);
}

// Called when the game starts or when spawned
void ALantern::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALantern::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


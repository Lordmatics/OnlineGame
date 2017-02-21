// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Pickups/Pickups.h"

// Sets default values
APickups::APickups()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRootComponent"));
	RootComponent = MyRoot;

	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyStaticMeshComponent"));
	MyStaticMesh->bGenerateOverlapEvents = true;
	MyStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &APickups::OnTriggerEnter);
	MyStaticMesh->SetupAttachment(MyRoot);
}

// Called when the game starts or when spawned
void APickups::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickups::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickups::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerEnter_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

/** Call this Super::Function at the end of the overriden logic - since this handles the logic, that every pickup will do*/
void APickups::TriggerEnter_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Adding in possiblity to move effect through code if lacking animations
	// And doppler the sound but probably wont use it.
	const FVector SpawnLocation = GetActorLocation();
	PlayPickUpSound(SpawnLocation);
	PlayPickUpEffect(SpawnLocation);
	// Might wanna delay this by 1s, and just "Fake Destroy" StaticMesh, we'll see
	Destroy();
}

void APickups::PlayPickUpSound(const FVector& Location)
{
	if (PickUpSound == nullptr) return;
	UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, Location, SoundVolume);
}

void APickups::PlayPickUpEffect(const FVector& Location)
{
	if (PickUpEffect == nullptr) return;
	UGameplayStatics::SpawnEmitterAtLocation(this, PickUpEffect, Location);
}

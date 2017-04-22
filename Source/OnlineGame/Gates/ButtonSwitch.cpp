// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "ButtonSwitch.h"
#include "OnlineGameCharacter.h"

// Sets default values
AButtonSwitch::AButtonSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyStaticMesh"));
	MyStaticMesh->SetupAttachment(MyRoot);

	MyTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("MyTrigger"));
	MyTrigger->bGenerateOverlapEvents = true;
	MyTrigger->OnComponentBeginOverlap.AddDynamic(this, &AButtonSwitch::OnTriggerEnter);
	MyTrigger->SetupAttachment(MyRoot);

	bPressed = false;
}

// Called when the game starts or when spawned
void AButtonSwitch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AButtonSwitch::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AButtonSwitch::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Other Actor: %s"), *OtherActor->GetName());

	}
	AOnlineGameCharacter* Character = Cast<AOnlineGameCharacter>(OtherActor);
	if (Character != nullptr)
	{
		SetPressed(true);
	}
}

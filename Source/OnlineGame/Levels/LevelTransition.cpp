// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "LevelTransition.h"
#include "OnlineGameGameMode.h"
#include "OnlineGameCharacter.h"
#include "Engine.h"

// Sets default values
ALevelTransition::ALevelTransition()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyStaticMesh"));
	MyStaticMesh->SetupAttachment(MyRoot);

	MyTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("MyTrigger"));
	MyTrigger->bGenerateOverlapEvents = true;
	MyTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransition::OnTriggerEnter);
	MyTrigger->OnComponentEndOverlap.AddDynamic(this, &ALevelTransition::OnTriggerExit);
	MyTrigger->SetupAttachment(MyStaticMesh);
}

// Called when the game starts or when spawned
void ALevelTransition::BeginPlay()
{
	Super::BeginPlay();
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	MyGameMode = Cast<AOnlineGameGameMode>(World->GetAuthGameMode());
}

// Called every frame
void ALevelTransition::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("Counter: %d"), Counter));
		if (MyGameMode != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("Counter: %d"), MyGameMode->GetPCInGame()));
		}
	}
}

void ALevelTransition::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnlineGameCharacter* Char = Cast<AOnlineGameCharacter>(OtherActor);
	if (Char != nullptr)
	{
		Counter++;
		UE_LOG(LogTemp, Warning, TEXT("Counter Increased: %d"), Counter);
	}
}

void ALevelTransition::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AOnlineGameCharacter* Char = Cast<AOnlineGameCharacter>(OtherActor);
	if (Char != nullptr)
	{
		Counter--;
		UE_LOG(LogTemp, Warning, TEXT("Counter Decreased: %d"), Counter);
	}
}

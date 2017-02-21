// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Tresure/TreasureChest.h"
#include "OnlineGameCharacter.h"
#include "Classes/Animation/AnimInstance.h"
#include "Pickups/Pickups.h"

// Sets default values
ATreasureChest::ATreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRootComponent"));
	RootComponent = MyRoot;

	GoldSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("GoldSpawn"));
	GoldSpawn->SetupAttachment(MyRoot);

	MyTreasureChest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MyTreasureChest"));
	MyTreasureChest->bGenerateOverlapEvents = true;
	MyTreasureChest->SetHiddenInGame(false);
	MyTreasureChest->SetupAttachment(MyRoot);

	MyTreasureChestEnd = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MyTreasureChestEnd"));
	MyTreasureChestEnd->bGenerateOverlapEvents = true;
	MyTreasureChestEnd->SetHiddenInGame(true);
	MyTreasureChestEnd->SetupAttachment(MyRoot);

	MyTriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("MyTriggerZone"));
	MyTriggerZone->bGenerateOverlapEvents = true;
	MyTriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ATreasureChest::OnTriggerEnter);
	MyTriggerZone->SetupAttachment(MyTreasureChest);

	bTreasureChestIsOpen = false;
}

// Called when the game starts or when spawned
void ATreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnGold();
}

// Called every frame
void ATreasureChest::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATreasureChest::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnTriggerEnter"));
	// Early Exit if chest is already / mid openning
	if (bTreasureChestIsOpen) return;

	AOnlineGameCharacter* OnlineCharacter = Cast<AOnlineGameCharacter>(OtherActor);
	if (OnlineCharacter != nullptr)
	{
		if (!OnlineCharacter->HasKeys()) return;
		UE_LOG(LogTemp, Warning, TEXT("Character Valid"));
		UWorld* const World = GetWorld();
		if (World == nullptr) return;

		if (MyTreasureChest != nullptr)
		{
			UAnimInstance* AnimInstance = MyTreasureChest->GetAnimInstance();
			if (AnimInstance != nullptr && OpenAnim != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Animation Approved"));

				bTreasureChestIsOpen = true;
				// At the end of animation - turn collisions off -
				// So Player can absorb the gold
				float AnimDuration = AnimInstance->Montage_Play(OpenAnim, PlaySpeed);
				FTimerHandle OpenChestHandle;
				World->GetTimerManager().SetTimer(OpenChestHandle, this, &ATreasureChest::OnTreasureChestOpenned, AnimDuration + AnimDurationOffset);
				UE_LOG(LogTemp, Warning, TEXT("%s has %d keys before using one"), *OnlineCharacter->GetName(), OnlineCharacter->GetKeyCount());
				OnlineCharacter->UseKey();
				UE_LOG(LogTemp, Warning, TEXT("%s has %d keys after using one"), *OnlineCharacter->GetName(), OnlineCharacter->GetKeyCount());
			}
		}
	}
}

void ATreasureChest::OnTreasureChestOpenned()
{
	UE_LOG(LogTemp, Warning, TEXT("Timer Ran"));

	if (MyTreasureChest != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Timer Succeeded"));

		MyTreasureChest->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MyTreasureChest->SetHiddenInGame(true);
	}
	if (MyTreasureChestEnd != nullptr)
	{
		MyTreasureChestEnd->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MyTreasureChestEnd->SetHiddenInGame(false);
	}
}

void ATreasureChest::SpawnGold()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (GoldClass == nullptr) return;
	//const FVector SpawnLocation = GetActorLocation() + SpawnOffset;
	const FRotator SpawnRotation = GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APickups* Gold = World->SpawnActor<APickups>(GoldClass, GoldSpawn->GetComponentLocation(), SpawnRotation, SpawnParams);
	if (Gold != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gold Spawned"));

	}
}
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
	if(MyGameMode != nullptr)
		PlayersInGameCount = MyGameMode->GetPCInGame();

	//create dynamic material anywhere u like, Constructor or anywhere .
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(Material, this);
	//set paramater with Set***ParamaterValue
	DynMaterial->SetScalarParameterValue("Glow", PanelGlow);
	MyStaticMesh->SetMaterial(0, DynMaterial);

	
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
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("PlayersInGame: %d"), MyGameMode->GetPCInGame()));
		}
	}
	if (bEveryoneInZone)
	{
		PanelGlow += DeltaTime * (1 / LevelChargeDelay);
		PanelGlow = FMath::Clamp(PanelGlow, 0.0f, 1.0f);
	}
	else
	{
		PanelGlow -= DeltaTime * (1 / LevelChargeDelay);
		PanelGlow = FMath::Clamp(PanelGlow, 0.0f, 1.0f);
	}
}

void ALevelTransition::ChangeServerLevel()
{
	if (HasAuthority())
	{
		UWorld* const World = GetWorld();
		if (World == nullptr) return;
		//APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
		//World->ServerTravel("/Game/Maps/Level_002", false);
		UGameplayStatics::OpenLevel(World, FName("MainMenu"));
		//World->ServerTravel("/Game/Maps/Lobby", false);

		//World->Exec(World, (TEXT("servertravel Level_002")));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Has Authority")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Didnt have Authority")));
	}
}

void ALevelTransition::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnlineGameCharacter* Char = Cast<AOnlineGameCharacter>(OtherActor);
	if (Char != nullptr)
	{
		Counter++;
		if (PlayersInGameCount > 0)
		{
			if (Counter >= PlayersInGameCount)
			{
				BeginLevelSwap();
				//ChangeServerLevel();
			}
		}

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
		bEveryoneInZone = false;
		ClearTransition();

	}
}

void ALevelTransition::BeginLevelSwap()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	bEveryoneInZone = true;
	World->GetTimerManager().SetTimer(TransitionHandle, this, &ALevelTransition::ShowStats, LevelChargeDelay, false);
}

void ALevelTransition::ShowStats()
{
	if (HasAuthority())
	{
		UWorld* const World = GetWorld();
		if (World == nullptr) return;
		//APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
		//World->ServerTravel("/Game/Maps/Level_002", false);
		UGameplayStatics::OpenLevel(World, FName("MainMenu"));
		//UGameplayStatics::OpenLevel(World, FName("AfterLevelStats"));

	}
}

void ALevelTransition::ClearTransition()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	World->GetTimerManager().ClearTimer(TransitionHandle);
}
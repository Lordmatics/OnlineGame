// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "OnlineGame.h"
#include "OnlineGameGameMode.h"
#include "OnlineGameCharacter.h"
#include "OnlineGamePlayerController.h"
#include "GameInstance/OnlineGameInstance.h"
#include "EngineUtils.h"

AOnlineGameGameMode::AOnlineGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void AOnlineGameGameMode::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);
	// Add New Player Controller to Array
	PlayerControllers.Add(NewPC);
}

void AOnlineGameGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AOnlineGamePlayerController* MyPlayerController = Cast<AOnlineGamePlayerController>(Exiting);
	if (MyPlayerController != nullptr)
	{
		UOnlineGameInstance* MyGameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
		if (MyGameInstance != nullptr)
		{
			// Disconnect this player controller 
			// From the existing session
			MyGameInstance->DestroySessionCaller(MyPlayerController);
		}
	}
}

//void AOnlineGameGameMode::ServerRespawnPlayerNetwork(APlayerController* _PlayerController, ACharacter* _PlayerCharacter)
//{
//	UE_LOG(LogTemp, Warning, TEXT("OnlineGameMode: RespawnPlayerNetwork Ran"));
//	// If World Failed for some reason 
//	// Don't Proceed
//	UWorld* const World = GetWorld();
//	if (World == nullptr) return;
//	
//	for (TActorIterator<APlayerStart> ActorItr(World); ActorItr; ++ActorItr)
//	{
//		// Fill in Respawn PlayerStarts Array
//		PlayerStarts.Add(*ActorItr);
//	}
//	
//	// For Respawning : Make sure former player 
//	// Gets destroyed if they were
//	// Previously alive
//	APawn* ControllerPawn = _PlayerController->GetPawn();
//	if (ControllerPawn != nullptr)
//	{
//		ControllerPawn->Destroy();
//	}
//
//	// Find Random Spawn Point
//	// Respawn There.
//	int SpawnIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);
//	const FTransform SpawnTransform = PlayerStarts[SpawnIndex]->GetActorTransform();
//	ACharacter* SpawnedCharacter = World->SpawnActor<ACharacter>(_PlayerCharacter->GetClass(), SpawnTransform);
//	if (SpawnedCharacter != nullptr)
//	{
//		_PlayerController->Possess(SpawnedCharacter);
//	}
//}
//
//void AOnlineGameGameMode::ServerRespawnPlayerNetwork_Implementation(APlayerController* _PlayerController, ACharacter* _PlayerCharacter)
//{
//	ServerRespawnPlayerNetwork(_PlayerController, _PlayerCharacter);
//}
//
//bool AOnlineGameGameMode::ServerRespawnPlayerNetwork_Validate(APlayerController* _PlayerController, ACharacter* _PlayerCharacter)
//{
//	return true;
//}

void AOnlineGameGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AOnlineGameGameMode, 5.0f);
}
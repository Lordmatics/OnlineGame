// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "OnlineGameLobbyGameMode.h"
#include "EngineUtils.h"
#include "GameInstance/OnlineGameInstance.h"
#include "Lobby/OnlineGameLobbyPlayerController.h"
#include "OnlineGameCharacter.h"

void AOnlineGameLobbyGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AOnlineGameGameMode, 5.0f);
}

// Initial Setup - Handles lobby setup on player controller
void AOnlineGameLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	// Only for Sevrver
	//if (Role == ROLE_Authority)
	//{
	//	if(NewPlayer != nullptr)
	//		PlayerControllersArray.Add(NewPlayer);
	//	UWorld* const World = GetWorld();
	//	if (World != nullptr)
	//	{
	//		for (TActorIterator<APlayerStart> ActorItr(World); ActorItr; ++ActorItr)
	//		{
	//			// Fill in Respawn PlayerStarts Array
	//			PlayerStartsArray.Add(*ActorItr);
	//		}
	//		UOnlineGameInstance* MyGameInstance = Cast<UOnlineGameInstance>(World->GetGameInstance());
	//		if (MyGameInstance != nullptr)
	//		{
	//			ServerName = MyGameInstance->GetServerName();
	//			MaxNumberOfPlayers = MyGameInstance->GetMaxNumOfPlayers();
	//			AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(NewPlayer);
	//			if (LobbyPC != nullptr)
	//			{
	//				LobbyPC->InitialSetup();
	//				LobbyPC->SetupLobbyMenu(ServerName);
	//				LobbyPC->UpdateLobbySettings(GameModeMapImage, GameModeMapName, GameModeMapTime);
	//				RespawnPlayers(LobbyPC);
	//			}
	//		}
	//	}
	//}
}

// Removes player from lists, and updates arrays
void AOnlineGameLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	//for (size_t i = 0; i < PlayerControllersArray.Num(); i++)
	//{
	//	if (PlayerControllersArray[i] == Exiting)
	//	{
	//		AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(PlayerControllersArray[i]);
	//		if (LobbyPC != nullptr)
	//		{
	//			bAvailableCharactersArray[LobbyPC->GetSelectedCharacterIndex()] = false;
	//			PlayerControllersArray.RemoveAt(i);
	//			ConnectedPlayersArray.RemoveAt(i);
	//			EveryoneUpdate();
	//			break;
	//		}
	//	}
	//}
}

// Spawn player as base character when arriving into the lobby - Update all lobbies
void AOnlineGameLobbyGameMode::RespawnPlayers_Implementation(APlayerController* _PlayerController)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (_PlayerController != nullptr)
	{
		APawn* ControllerPawn = _PlayerController->GetPawn();
		if (ControllerPawn != nullptr)
		{
			ControllerPawn->Destroy();
			if (PlayerStartsArray.Num() > 0)
			{
				int SpawnIndex = FMath::RandRange(0, PlayerStartsArray.Num() - 1);

				if (PlayerStartsArray[SpawnIndex] != nullptr)
				{
					FTransform SpawnTransform = PlayerStartsArray[SpawnIndex]->GetActorTransform();
					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					if (CharactersArray.Num() > 0)
					{
						if (CharactersArray[0] != nullptr)
						{
							AOnlineGameCharacter* BaseCharacter = World->SpawnActor<AOnlineGameCharacter>(CharactersArray[0], SpawnTransform, SpawnParams);
							if (BaseCharacter != nullptr)
							{
								_PlayerController->Possess(BaseCharacter);
								EveryoneUpdate();
							}
						}
					}
				}
			}
		}
	}
}

bool AOnlineGameLobbyGameMode::RespawnPlayers_Validate(APlayerController* _PlayerController)
{
	if (_PlayerController != nullptr)
	{
		return true;
	}
	else
		return false;
}

// Updates all Connected players lobby information when called
void AOnlineGameLobbyGameMode::EveryoneUpdate_Implementation()
{
	CurrentNumberOfPlayers = PlayerControllersArray.Num();
	if (CurrentNumberOfPlayers > 0)
	{
		ConnectedPlayersArray.Empty();
		for (APlayerController* PC : PlayerControllersArray)
		{
			AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(PC);
			if (LobbyPC != nullptr)
			{
				ConnectedPlayersArray.Add(LobbyPC->GetPlayerSettings());
				LobbyPC->UpdateNumberOfPlayers(CurrentNumberOfPlayers, MaxNumberOfPlayers);
			}
		}
		for (APlayerController* PC2 : PlayerControllersArray)
		{
			AOnlineGameLobbyPlayerController* LobbyPC2 = Cast<AOnlineGameLobbyPlayerController>(PC2);
			if (LobbyPC2 != nullptr)
			{
				LobbyPC2->AddPlayerInfo(ConnectedPlayersArray);
				LobbyPC2->UpdateAvailableCharacters(bAvailableCharactersArray);
				AddToKickList();
			}
		}
		for (FMyPlayerInfo PlayerInf : ConnectedPlayersArray)
		{
			if (PlayerInf.CharacterClass == CharactersArray[0])
			{
				bCanWeStart = false;
				break;
			}
			else
			{
				bCanWeStart = true;
			}
		}
	}
}

bool AOnlineGameLobbyGameMode::EveryoneUpdate_Validate()
{
	return true;
}

// Server has made changes , tell each client to update their lobby information
void AOnlineGameLobbyGameMode::ServerUpdateGameSettings_Implementation(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime, int _MapID)
{
	GameModeMapImage = _MapImage;
	GameModeMapName = _MapName;
	GameModeMapTime = _MapTime;
	GameModeMapID = _MapID;
	for (APlayerController* PC : PlayerControllersArray)
	{
		AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(PC);
		if (LobbyPC != nullptr)
		{
			LobbyPC->UpdateLobbySettings(GameModeMapImage, GameModeMapName, GameModeMapTime);
		}
	}
}

bool AOnlineGameLobbyGameMode::ServerUpdateGameSettings_Validate(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime, int _MapID)
{
	return true;
}

// Launch game with selected map
void AOnlineGameLobbyGameMode::LaunchTheGame()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	// Coz its online, all owners are index 0
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	if (PC != nullptr)
	{
		switch (GameModeMapID)
		{
			case 0:
			{
				World->Exec(World, TEXT("servertravel /Game/Maps/Level_001"));
				break;
			}
			case 1:
			{
				World->Exec(World, TEXT("servertravel /Game/Maps/Level_002"));
				break;
			}
		}
	}
}

// Removes player from lists, and updates arrays
void AOnlineGameLobbyGameMode::YouHaveBeenKicked_Implementation(int _PlayerID)
{
	AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(PlayerControllersArray[_PlayerID]);
	if (LobbyPC != nullptr)
	{
		LobbyPC->Kicked();
	}
}

bool AOnlineGameLobbyGameMode::YouHaveBeenKicked_Validate(int _PlayerID)
{
	return true;
}

// Adds player to kick list, to handle the error, when kick window is open when someone new joins
// Auto refresh it
void AOnlineGameLobbyGameMode::AddToKickList()
{
	AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (LobbyPC != nullptr)
	{
		// NEED TO MAKE UUSERWIDGET CUSTOM CLASSES
		//if (LobbyPC->GetLobbyWidgetInstance()->GameSettings.IsVisible())
		//{
		//	LobbyPC->GetLobbyWidgetInstance()->GameSettings.FillPlayersWindow();
		//}
	}
}

// Change Character
void AOnlineGameLobbyGameMode::SwapCharacters_Implementation(APlayerController* _PlayerController, TSubclassOf<class AOnlineGameCharacter> _CharacterClass, bool _bChangeStatus)
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	if (!_bChangeStatus)
	{
		APawn* ControlledPawn = _PlayerController->GetPawn();
		if (ControlledPawn != nullptr)
		{
			ControlledPawn->Destroy();
			if (PlayerStartsArray.Num() > 0)
			{
				int SpawnIndex = FMath::RandRange(0, PlayerStartsArray.Num() - 1);

				if (PlayerStartsArray[SpawnIndex] != nullptr)
				{
					FTransform SpawnTransform = PlayerStartsArray[SpawnIndex]->GetActorTransform();
					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					if (_CharacterClass != nullptr)
					{
						AOnlineGameCharacter* SpawnedCharacter = World->SpawnActor<AOnlineGameCharacter>(_CharacterClass, SpawnTransform, SpawnParams);
						if (SpawnedCharacter != nullptr)
						{
							_PlayerController->Possess(SpawnedCharacter);
						}
					}	
				}
			}
		}
	}
}

bool AOnlineGameLobbyGameMode::SwapCharacters_Validate(APlayerController* _PlayerController, TSubclassOf<class AOnlineGameCharacter> _CharacterClass, bool _bChangeStatus)
{
	return true;
}
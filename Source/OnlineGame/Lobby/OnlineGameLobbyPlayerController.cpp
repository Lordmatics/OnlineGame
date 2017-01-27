// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "OnlineGameLobbyPlayerController.h"
#include "Lobby/OnlineGameLobbyGameMode.h"
#include "SaveGame/Saving.h"

void AOnlineGameLobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Destroy Session
}

void AOnlineGameLobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AOnlineGameGameMode, 5.0f);
}

void AOnlineGameLobbyPlayerController::SaveGameCheck()
{
	const FString& SlotName = PlayerSaveString;
	bool bSaveExist = UGameplayStatics::DoesSaveGameExist(SlotName, 0);
	if (bSaveExist)
	{
		LoadGame();
		SaveGame();
	}
	else
	{
		SaveGame();
	}
}

void AOnlineGameLobbyPlayerController::SaveGame()
{
	const FString& SlotName = PlayerSaveString;
	if (PlayerSaveGameRef == nullptr)
	{
		USaving* SaveData = Cast<USaving>(UGameplayStatics::CreateSaveGameObject(PlayerSaveGameRef->GetClass()));
		if (SaveData != nullptr)
		{
			PlayerSaveGameRef = SaveData;
		}
	}

	PlayerSaveGameRef->SetPlayerStruct(ThePlayerSettings);
	UGameplayStatics::SaveGameToSlot(PlayerSaveGameRef, SlotName, 0);
}

void AOnlineGameLobbyPlayerController::LoadGame()
{
	const FString& SlotName = PlayerSaveString;
	USaving* SaveData = Cast<USaving>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (SaveData != nullptr)
	{
		ThePlayerSettings.PlayerName = SaveData->GetPlayerStruct().PlayerName;
		ThePlayerSettings.PlayerLogo = SaveData->GetPlayerStruct().PlayerLogo;
		// Potentially, load stats here too, not sure yet
	}
}

void AOnlineGameLobbyPlayerController::CharacterCheck()
{
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	AOnlineGameLobbyGameMode* LobbyGameMode = Cast<AOnlineGameLobbyGameMode>(World->GetAuthGameMode());
	if (LobbyGameMode == nullptr) return;

	if (SelectedCharacterIndex == 0)
	{
		LobbyGameMode->SetAvailableCharactersArrayElement(SelectedCharacterIndex, false);
	}
	else
	{
		if (LobbyGameMode->GetAvailableCharactersArray()[SelectedCharacterIndex] != 0)
		{
			LobbyGameMode->SetAvailableCharactersArrayElement(SelectedCharacterIndex, true);
		}
	}
	AssignPlayer(LobbyGameMode->GetCharactersArray()[SelectedCharacterIndex]->GetClass(), PlayerLogo);
}

void AOnlineGameLobbyPlayerController::InitialSetup_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: InitialSetup Ran"));
	SaveGameCheck();
	CallUpdate(ThePlayerSettings, false);
}

bool AOnlineGameLobbyPlayerController::InitialSetup_Validate()
{
	return true;
}

void AOnlineGameLobbyPlayerController::SetupLobbyMenu_Implementation(const FString& _ServerName)
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: SetupLobbyMenu Ran"));
	bShowMouseCursor = true;
	// Create Lobby Menu Widget , pass in _ServerName, then set this widget to variable
	// Add to Viewport
	// Set Input mode to Game + UI

}

bool AOnlineGameLobbyPlayerController::SetupLobbyMenu_Validate(const FString& _ServerName)
{
	return true;
}

void AOnlineGameLobbyPlayerController::CallUpdate_Implementation(FMyPlayerInfo _PlayerSettings, bool bChangedStatus)
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: CallUpdate Ran"));
	ThePlayerSettings = _PlayerSettings;
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	AOnlineGameLobbyGameMode* LobbyGameMode = Cast<AOnlineGameLobbyGameMode>(World->GetAuthGameMode());
	if (LobbyGameMode != nullptr)
	{
		//LobbyGameMode->SwapCharacters(this, ThePlayerSettings.CharacterClass, bChangedStatus);
		//LobbyGameMode->EveryoneRunUpdate();
	}
}

bool AOnlineGameLobbyPlayerController::CallUpdate_Validate(FMyPlayerInfo _PlayerSettings, bool bChangedStatus)
{
	return true;
}

void AOnlineGameLobbyPlayerController::AddPlayerInfo_Implementation(const TArray<FMyPlayerInfo>& _ConnectedPlayerInfoArray)
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: AddPlayerInfo Ran"));
	ConnectedPlayersInfoArray = _ConnectedPlayerInfoArray;
	// If Lobby Menu Widget is valid
	// Clear Player List
	size_t Length = ConnectedPlayersInfoArray.Num();
	for (size_t i = 0; i < Length; i++)
	{
		// Lobby Menu Widget -> UpdatePlayerWindow(ConnectedPlayersInfoArray[i]);
	}
}

bool AOnlineGameLobbyPlayerController::AddPlayerInfo_Validate(const TArray<FMyPlayerInfo>& _ConnectedPlayerInfoArray)
{
	return true;
}

void AOnlineGameLobbyPlayerController::UpdateLobbySettings_Implementation(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime)
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: UpdateLobbySettings Ran"));
	// Lobby Menu Widget -> Set Map Image
	// And Name
	// And Time
	// ETC
}

bool AOnlineGameLobbyPlayerController::UpdateLobbySettings_Validate(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime)
{
	return true;
}

void AOnlineGameLobbyPlayerController::ShowLoadingScreen_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: Show LoadingScreen Ran"));
	// Remove Lobby Menu Widget from Parent
	// Create Loading Screen Widget
	// Add to Viewport
}

bool AOnlineGameLobbyPlayerController::ShowLoadingScreen_Validate()
{
	return true;
}

void AOnlineGameLobbyPlayerController::UpdateNumberOfPlayers_Implementation(int CurrentNum, int TotalNum)
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: Update Num Of Players Ran"));
	// If Lobby Menu Widget is Valid
	// Set Text to Cur of Total 
}

bool AOnlineGameLobbyPlayerController::UpdateNumberOfPlayers_Validate(int CurrentNum, int TotalNum)
{
	return true;
}

void AOnlineGameLobbyPlayerController::Kicked_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: Kicked Ran"));
	UGameplayStatics::OpenLevel(this, FName("MainMenu"));
	// Destroy Session
}

bool AOnlineGameLobbyPlayerController::Kicked_Validate()
{
	return true;
}

void AOnlineGameLobbyPlayerController::UpdateAvailableCharacters_Implementation(const TArray<bool>& _AvailCharacters)
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: UpdateAvailableCharacters Ran"));
	bAvailableCharactersArray = _AvailCharacters;
	// If LobbyMenuWidget is Valid
	// Use Character Select and Character Array from Widget
	size_t Length = 5; // LobbyMenuWidget.Array.Num();
	for (size_t i = 0; i < Length; i++)
	{
		//LobbyArray[i]->SetEnabled(!bAvailableCharactersArray[LobbyArray[i]]);
	}
}

bool AOnlineGameLobbyPlayerController::UpdateAvailableCharacters_Validate(const TArray<bool>& _AvailCharacters)
{
	return true;
}

void AOnlineGameLobbyPlayerController::AssignSelectedCharacter_Implementation(int _CharID, UTexture2D* _CharImage)
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: AssignSelectedCharacter Ran"));
	PreviousSelectionIndex = SelectedCharacterIndex;
	PlayerLogo = _CharImage;
	SelectedCharacterIndex = _CharID;
	CharacterCheck();

}

bool AOnlineGameLobbyPlayerController::AssignSelectedCharacter_Validate(int _CharID, UTexture2D* _CharImage)
{
	return true;
}

void AOnlineGameLobbyPlayerController::AssignPlayer_Implementation(TSubclassOf<class ACharacter> CharClass, UTexture2D* _CharImage)
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: AssignPlayer Ran"));
	ThePlayerSettings.CharacterClass = CharClass;
	ThePlayerSettings.PlayerLogo = _CharImage;
	SaveGame();
	CallUpdate(ThePlayerSettings, false);
}

bool AOnlineGameLobbyPlayerController::AssignPlayer_Validate(TSubclassOf<class ACharacter> CharClass, UTexture2D* _CharImage)
{
	return true;
}

void AOnlineGameLobbyPlayerController::GetMyChatMessage_Implementation(const FString& TextToSend)
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: GetMyChatMessage Ran"));
	SenderText = TextToSend;
	SenderName = ThePlayerSettings.PlayerName;
	UWorld* const World = GetWorld();
	if (World == nullptr) return;
	AOnlineGameLobbyGameMode* LobbyGameMode = Cast<AOnlineGameLobbyGameMode>(World->GetAuthGameMode());
	if (LobbyGameMode != nullptr) 
	{
		size_t Length = LobbyGameMode->GetPlayerControllersArray().Num();
		if (Length <= 0) return;
		for (size_t i = 0; i < Length; i++)
		{
			AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(LobbyGameMode->GetPlayerControllersArray()[i]);
			if (LobbyPC != nullptr)
			{
				LobbyPC->UpdateMyChat(SenderName, SenderText);
			}
		}
	}
}

bool AOnlineGameLobbyPlayerController::GetMyChatMessage_Validate(const FString& TextToSend)
{
	return true;
}

void AOnlineGameLobbyPlayerController::UpdateMyChat_Implementation(const FString& _SenderName, const FString& _SenderText)
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: UpdateChat Ran"));
	// LobbyMenu Widget -> Chat Window -> UpdateChatWindow(SenderName,SenderText);
}

bool AOnlineGameLobbyPlayerController::UpdateMyChat_Validate(const FString& _SenderName, const FString& _SenderText)
{
	return true;
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "OnlineGameLobbyPlayerController.h"
#include "Lobby/OnlineGameLobbyGameMode.h"
#include "SaveGame/Saving.h"
#include "Runtime/UMG/Public/UMG.h"

// OKAY
// THIS CLASS
// ALL VARIABLES USED FROM C++ IN BP
// FUNCTIONS, DONE IN PURE BP - 
// SetupLobbyMenu(const FString& _ServerName);
// AddPlayerInfo(const TArray<FMyPlayerInfo>& _ConnectedPlayerInfoArray);
// UpdateLobbySettings(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime);
// ShowLoadingScreen();
// UpdateNumberOfPlayers(int CurrentNum, int TotalNum);
// InitialSetup_Implementation()
// CallUpdate_Implementation(FMyPlayerInfo _PlayerSettings, bool bChangedStatus)
// AssignPlayer_Implementation(TSubclassOf<class ACharacter> CharClass, UTexture2D* _CharImage)
// Kicked();
// UpdateAvailableCharacters(const TArray<bool>& _AvailCharacters);
// GetMyChatMessage(const FString& TextToSend);
// UpdateMyChat(const FString& _SenderName, const FString& _SenderText);
// CharacterCheckCode()
// AssignSelectedCharacter_Implementation(int _CharID, UTexture2D* _CharImage)

// Overall, all this class in code does now is,
// Check if data exists
// Save Data
// Load Data

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

void AOnlineGameLobbyPlayerController::SaveGameCheckCode()
{
	const FString& SlotName = PlayerSaveString;
	bool bSaveExist = UGameplayStatics::DoesSaveGameExist(SlotName, 0);
	if (bSaveExist)
	{
		LoadGameCode();
		SaveGameCode();
	}
	else
	{
		SaveGameCode();
	}
}

void AOnlineGameLobbyPlayerController::SaveGameCode()
{
	if (SaveClass == nullptr) return;
	const FString& SlotName = PlayerSaveString;
	if (PlayerSaveGameRef == nullptr)
	{
		USaving* SaveData = Cast<USaving>(UGameplayStatics::CreateSaveGameObject(SaveClass));// PlayerSaveGameRef->GetClass()));
		if (SaveData != nullptr)
		{
			PlayerSaveGameRef = SaveData;
		}
	}

	PlayerSaveGameRef->SetPlayerStruct(ThePlayerSettings);
	UGameplayStatics::SaveGameToSlot(PlayerSaveGameRef, SlotName, 0);
}

void AOnlineGameLobbyPlayerController::LoadGameCode()
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

void AOnlineGameLobbyPlayerController::CharacterCheckCode()
{
	//UWorld* const World = GetWorld();
	//if (World == nullptr) return;
	//AOnlineGameLobbyGameMode* LobbyGameMode = Cast<AOnlineGameLobbyGameMode>(World->GetAuthGameMode());
	//if (LobbyGameMode == nullptr) return;

	//if (SelectedCharacterIndex == 0)
	//{
	//	LobbyGameMode->SetAvailableCharactersArrayElement(SelectedCharacterIndex, false);
	//}
	//else
	//{
	//	if (LobbyGameMode->GetAvailableCharactersArray()[SelectedCharacterIndex] != 0)
	//	{
	//		LobbyGameMode->SetAvailableCharactersArrayElement(SelectedCharacterIndex, true);
	//	}
	//}
	//AssignPlayer(LobbyGameMode->GetCharactersArray()[SelectedCharacterIndex]->GetClass(), PlayerLogo);
}

// Check for save game, then update the Server
void AOnlineGameLobbyPlayerController::InitialSetup_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: InitialSetup Ran"));
	//SaveGameCheckCode();
	//CallUpdate(ThePlayerSettings, false);
}

bool AOnlineGameLobbyPlayerController::InitialSetup_Validate()
{
	return true;
}

// Setup and Display Lobby Menu
void AOnlineGameLobbyPlayerController::SetupLobbyMenu_Implementation(const FString& _ServerName)
{
	// NOW DONE IN BP

	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: SetupLobbyMenu Ran"));
	//// Create Lobby Menu Widget , pass in _ServerName, then set this widget to variable
	//// Add to Viewport
	//// Set Input mode to Game + UI
	//if (TestLobbyWidgetClass != nullptr)
	//{
	//	if (!TestLobbyWidgetInstance)
	//	{
	//		// MIGHT NEED TO MAKE A CHILD OF UUSERWIDGET FOR SERVER NAME PASS IN
	//		TestLobbyWidgetInstance = CreateWidget<UUserWidget>(this, TestLobbyWidgetClass);
	//		if (!TestLobbyWidgetInstance->GetIsVisible())
	//		{
	//			TestLobbyWidgetInstance->AddToViewport();
	//			FInputModeGameAndUI Mode;
	//			Mode.SetHideCursorDuringCapture(true);
	//			Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//			Mode.SetWidgetToFocus(TestLobbyWidgetInstance->GetCachedWidget());
	//			SetInputMode(Mode);
	//			bShowMouseCursor = true;
	//		}
	//	}
	//}
}

bool AOnlineGameLobbyPlayerController::SetupLobbyMenu_Validate(const FString& _ServerName)
{
	return true;
}

// Assigns the updated character selection / Tells everyone to update their lobby menus
void AOnlineGameLobbyPlayerController::CallUpdate_Implementation(FMyPlayerInfo _PlayerSettings, bool bChangedStatus)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: CallUpdate Ran"));
	//ThePlayerSettings = _PlayerSettings;
	//UWorld* const World = GetWorld();
	//if (World == nullptr) return;
	//AOnlineGameLobbyGameMode* LobbyGameMode = Cast<AOnlineGameLobbyGameMode>(World->GetAuthGameMode());
	//if (LobbyGameMode != nullptr)
	//{
	//	//LobbyGameMode->SwapCharacters(this, ThePlayerSettings.CharacterClass, bChangedStatus);
	//	//LobbyGameMode->EveryoneRunUpdate();
	//}
}

bool AOnlineGameLobbyPlayerController::CallUpdate_Validate(FMyPlayerInfo _PlayerSettings, bool bChangedStatus)
{
	return true;
}

// Update player List when called
void AOnlineGameLobbyPlayerController::AddPlayerInfo_Implementation(const TArray<FMyPlayerInfo>& _ConnectedPlayerInfoArray)
{
	// NOW DONE IN BP

	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: AddPlayerInfo Ran"));
	//ConnectedPlayersInfoArray = _ConnectedPlayerInfoArray;
	//// If Lobby Menu Widget is valid
	//if (TestLobbyWidgetInstance != nullptr)
	//{
	//	//TestLobbyWidgetInstance->ClearPlayerList();
	//	// Clear Player List
	//	size_t Length = ConnectedPlayersInfoArray.Num();
	//	for (size_t i = 0; i < Length; i++)
	//	{
	//		//TestLobbyWidgetInstance->UpdatePlayerWindow(ConnectedPlayersInfoArray[i]);
	//	}
	//}

}

bool AOnlineGameLobbyPlayerController::AddPlayerInfo_Validate(const TArray<FMyPlayerInfo>& _ConnectedPlayerInfoArray)
{
	return true;
}

// Update Lobby info on clients
void AOnlineGameLobbyPlayerController::UpdateLobbySettings_Implementation(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime)
{
	// NOW DONE IN BP

	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: UpdateLobbySettings Ran"));
	//// Lobby Menu Widget -> Set Map Image
	//// And Name
	//// And Time
	//// ETC
	//if (TestLobbyWidgetInstance != nullptr)
	//{
	//	//TestLobbyWidgetInstance->SetMapImage(_MapImage);
	//	//TestLobbyWidgetInstance->SetMapName(_MapName);
	//	//TestLobbyWidgetInstance->SetMapTime(_MapTime);
	//}
}

bool AOnlineGameLobbyPlayerController::UpdateLobbySettings_Validate(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime)
{
	return true;
}

// Replace Lobby with LoadingScreen
void AOnlineGameLobbyPlayerController::ShowLoadingScreen_Implementation()
{
	// NOW DONE IN BP

	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: Show LoadingScreen Ran"));
	//// Remove Lobby Menu Widget from Parent
	//// Create Loading Screen Widget
	//// Add to Viewport
	//if (TestLobbyWidgetInstance != nullptr)
	//{
	//	TestLobbyWidgetInstance->RemoveFromParent();
	//	UUserWidget* LoadingWidget = CreateWidget<UUserWidget>(this, LoadingScreenWidgetClass);
	//	if (LoadingWidget != nullptr)
	//	{
	//		LoadingWidget->AddToViewport();
	//	}
	//}
}

bool AOnlineGameLobbyPlayerController::ShowLoadingScreen_Validate()
{
	return true;
}

// Update Current Number of Players in Lobby
void AOnlineGameLobbyPlayerController::UpdateNumberOfPlayers_Implementation(int CurrentNum, int TotalNum)
{
	// NOW DONE IN BP

	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: Update Num Of Players Ran"));
	//// If Lobby Menu Widget is Valid
	//// Set Text to Cur of Total 
	//if (TestLobbyWidgetInstance != nullptr)
	//{
	//	//TestLobbyWidgetInstance->UpdatePlayersNumberDisplay(CurrentNum, TotalNum);
	//}
}

bool AOnlineGameLobbyPlayerController::UpdateNumberOfPlayers_Validate(int CurrentNum, int TotalNum)
{
	return true;
}

void AOnlineGameLobbyPlayerController::Kicked_Implementation()
{
	// NOW DONE IN BP

	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: Kicked Ran"));
	//UGameplayStatics::OpenLevel(this, FName("MainMenu"));
	// Destroy Session
}

bool AOnlineGameLobbyPlayerController::Kicked_Validate()
{
	return true;
}

// Update Available Characters - Make others inactive
void AOnlineGameLobbyPlayerController::UpdateAvailableCharacters_Implementation(const TArray<bool>& _AvailCharacters)
{
	// NOW DONE IN BP

	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: UpdateAvailableCharacters Ran"));
	//bAvailableCharactersArray = _AvailCharacters;
	//// If LobbyMenuWidget is Valid
	//if (TestLobbyWidgetInstance != nullptr)
	//{
	//	// Use Character Select and Character Array from Widget
	//	size_t Length = 5; // LobbyMenuWidget.Array.Num();
	//	for (size_t i = 0; i < Length; i++)
	//	{
	//		//LobbyArray[i]->SetEnabled(!bAvailableCharactersArray[LobbyArray[i]]);
	//	}
	//}
}

bool AOnlineGameLobbyPlayerController::UpdateAvailableCharacters_Validate(const TArray<bool>& _AvailCharacters)
{
	return true;
}

// Handle Character Selection / Swapping Etc
void AOnlineGameLobbyPlayerController::AssignSelectedCharacter_Implementation(int _CharID, UTexture2D* _CharImage)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: AssignSelectedCharacter Ran"));
	//PreviousSelectionIndex = SelectedCharacterIndex;
	//PlayerLogo = _CharImage;
	//SelectedCharacterIndex = _CharID;
	//CharacterCheckCode();

}

bool AOnlineGameLobbyPlayerController::AssignSelectedCharacter_Validate(int _CharID, UTexture2D* _CharImage)
{
	return true;
}

// Assign Selected Player - And Update for all Clients
void AOnlineGameLobbyPlayerController::AssignPlayer_Implementation(TSubclassOf<class ACharacter> CharClass, UTexture2D* _CharImage)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: AssignPlayer Ran"));
	//ThePlayerSettings.CharacterClass = CharClass;
	//ThePlayerSettings.PlayerLogo = _CharImage;
	//SaveGameCode();
	//CallUpdate(ThePlayerSettings, false);
}

bool AOnlineGameLobbyPlayerController::AssignPlayer_Validate(TSubclassOf<class ACharacter> CharClass, UTexture2D* _CharImage)
{
	return true;
}

// Process incoming chat message and tells all player controllers to run update chat
void AOnlineGameLobbyPlayerController::GetMyChatMessage_Implementation(const FString& TextToSend)
{
	// NOW DONE IN BP

	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: GetMyChatMessage Ran"));
	//SenderText = TextToSend;
	//SenderName = ThePlayerSettings.PlayerName;
	//UWorld* const World = GetWorld();
	//if (World == nullptr) return;
	//AOnlineGameLobbyGameMode* LobbyGameMode = Cast<AOnlineGameLobbyGameMode>(World->GetAuthGameMode());
	//if (LobbyGameMode != nullptr) 
	//{
	//	size_t Length = LobbyGameMode->GetPlayerControllersArray().Num();
	//	if (Length <= 0) return;
	//	for (size_t i = 0; i < Length; i++)
	//	{
	//		AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(LobbyGameMode->GetPlayerControllersArray()[i]);
	//		if (LobbyPC != nullptr)
	//		{
	//			LobbyPC->UpdateMyChat(SenderName, SenderText);
	//		}
	//	}
	//}
}

bool AOnlineGameLobbyPlayerController::GetMyChatMessage_Validate(const FString& TextToSend)
{
	return true;
}

// Update Display
void AOnlineGameLobbyPlayerController::UpdateMyChat_Implementation(const FString& _SenderName, const FString& _SenderText)
{
	// NOW DONE IN BP

	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: UpdateChat Ran"));
	//// LobbyMenu Widget -> Chat Window -> UpdateChatWindow(SenderName,SenderText);
	//if (TestLobbyWidgetInstance != nullptr)
	//{
	//	//TestLobbyWidgetInstance->UpdateChatWindow(_SenderName, _SenderText);
	//}
}

bool AOnlineGameLobbyPlayerController::UpdateMyChat_Validate(const FString& _SenderName, const FString& _SenderText)
{
	return true;
}

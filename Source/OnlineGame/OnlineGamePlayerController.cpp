// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "OnlineGamePlayerController.h"
#include "OnlineGameGameMode.h"
#include "SaveGame/Saving.h"

// OKAY
// THIS CLASS
// ALL VARIABLES USED FROM C++ IN BP
// FUNCTIONS, DONE IN PURE BP - 
// PassCharacterInfoToServer_Implementation(FMyPlayerInfo PlayerSettingsInfo)
// SetupMyChatWindow_Implementation()
// GetMyChatMessage_Implementation(const FString& TextToSend)
// UpdateMyChat_Implementation(const FString& _SenderName, const FString& _SenderText)

// So this class handles loading the players data pretty much

AOnlineGamePlayerController::AOnlineGamePlayerController()
{

}

void AOnlineGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Done in BP
	// Removes Widgets
	// Loads Data
	// Passes Info Through
	// Set Input mode
}

void AOnlineGamePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AOnlineGameGameMode, 5.0f);
}

void AOnlineGamePlayerController::PassCharacterInfoToServer_Implementation(FMyPlayerInfo PlayerSettingsInfo)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: PassCharacterInfoToServer Ran"));
	//PlayerSettings = PlayerSettingsInfo;
	//UWorld* const World = GetWorld();
	//if (World == nullptr) return;
	//AOnlineGameGameMode* MyGameMode = Cast<AOnlineGameGameMode>(World->GetAuthGameMode());
	//if (MyGameMode != nullptr)
	//{
	//	MyGameMode->ServerRespawnPlayerNetwork(this, PlayerSettings.CharacterClass);
	//	SetupMyChatWindow();
	//}
}

bool AOnlineGamePlayerController::PassCharacterInfoToServer_Validate(FMyPlayerInfo PlayerSettingsInfo)
{
	return true;
}

void AOnlineGamePlayerController::SetupMyChatWindow_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: SetupMyChatWindow Ran"));
	// TODO
	// Create GameplayChatWidget -> Set Variable
	// Add to Viewport -> Done in BP coz widgets

}

bool AOnlineGamePlayerController::SetupMyChatWindow_Validate()
{
	return true;
}

void AOnlineGamePlayerController::GetMyChatMessage_Implementation(const FString& TextToSend)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: GetMyChatMessage Ran"));
	//SenderText = TextToSend;
	//SenderName = PlayerSettings.PlayerName;
	//UWorld* const World = GetWorld();
	//if (World == nullptr) return;
	//AOnlineGameGameMode* MyGameMode = Cast<AOnlineGameGameMode>(World->GetAuthGameMode());
	//if (MyGameMode != nullptr)
	//{
	//	size_t Length = MyGameMode->PlayerControllers.Num();
	//	if (Length <= 0) return;
	//	for (size_t i = 0; i < Length; i++)
	//	{
	//		AOnlineGamePlayerController* PlayerController = Cast<AOnlineGamePlayerController>(MyGameMode->PlayerControllers[i]);
	//		if (PlayerController != nullptr)
	//		{
	//			PlayerController->UpdateMyChat(SenderName, SenderText);
	//		}
	//	}
	//}
}

bool AOnlineGamePlayerController::GetMyChatMessage_Validate(const FString& TextToSend)
{
	return true;
}

void AOnlineGamePlayerController::UpdateMyChat_Implementation(const FString& _SenderName, const FString& _SenderText)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: UpdateMyChat Ran"));
	// TODO
	// Get Chat Window variable from Widget
	// Call UpdateChatWindow on it
}

bool AOnlineGamePlayerController::UpdateMyChat_Validate(const FString& _SenderName, const FString& _SenderText)
{
	return true;
}

void AOnlineGamePlayerController::LoadGameViaSlot(const FString& SlotName, int32 UserIndex)
{
	USaving* MySave = Cast<USaving>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (MySave != nullptr)
	{
		PlayerSettings = MySave->GetPlayerStruct();
	}
}
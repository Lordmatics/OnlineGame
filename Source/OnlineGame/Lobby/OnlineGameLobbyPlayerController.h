// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MyStructs.h"
#include "OnlineGameLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API AOnlineGameLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Transient)
		FString PlayerSaveString = FString(TEXT("PlayerSaveString"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Transient)
		TSubclassOf<class USaving> PlayerSaveGameClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Transient)
		UTexture2D* PlayerLogo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		TArray<bool> bAvailableCharactersArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		TArray<FMyPlayerInfo> ConnectedPlayersInfoArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		FString SenderText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		FString SenderName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		int SelectedCharacterIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		int PreviousSelectionIndex;
public:

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};

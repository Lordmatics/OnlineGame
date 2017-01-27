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
		class USaving* PlayerSaveGameRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Transient)
		UTexture2D* PlayerLogo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		TArray<bool> bAvailableCharactersArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		FMyPlayerInfo ThePlayerSettings;

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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Utility Functions
	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void SaveGameCheck();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void LoadGame();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void CharacterCheck();

	// Server / Client functions


	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Client, WithValidation)
		void InitialSetup();
	virtual void InitialSetup_Implementation();
	virtual bool InitialSetup_Validate();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Client, WithValidation)
		void SetupLobbyMenu(const FString& _ServerName);
	virtual void SetupLobbyMenu_Implementation(const FString& _ServerName);
	virtual bool SetupLobbyMenu_Validate(const FString& _ServerName);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void CallUpdate(FMyPlayerInfo _PlayerSettings, bool bChangedStatus);
	virtual void CallUpdate_Implementation(FMyPlayerInfo _PlayerSettings, bool bChangedStatus);
	virtual bool CallUpdate_Validate(FMyPlayerInfo _PlayerSettings, bool bChangedStatus);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Client, WithValidation)
		void AddPlayerInfo(const TArray<FMyPlayerInfo>& _ConnectedPlayerInfoArray);
	virtual void AddPlayerInfo_Implementation(const TArray<FMyPlayerInfo>& _ConnectedPlayerInfoArray);
	virtual bool AddPlayerInfo_Validate(const TArray<FMyPlayerInfo>& _ConnectedPlayerInfoArray);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Client, WithValidation)
		void UpdateLobbySettings(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime);
	virtual void UpdateLobbySettings_Implementation(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime);
	virtual bool UpdateLobbySettings_Validate(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Client, WithValidation)
		void ShowLoadingScreen();
	virtual void ShowLoadingScreen_Implementation();
	virtual bool ShowLoadingScreen_Validate();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Client, WithValidation)
		void UpdateNumberOfPlayers(int CurrentNum, int TotalNum);
	virtual void UpdateNumberOfPlayers_Implementation(int CurrentNum, int TotalNum);
	virtual bool UpdateNumberOfPlayers_Validate(int CurrentNum, int TotalNum);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Client, WithValidation)
		void Kicked();
	virtual void Kicked_Implementation();
	virtual bool Kicked_Validate();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Client, WithValidation)
		void UpdateAvailableCharacters(const TArray<bool>& _AvailCharacters);
	virtual void UpdateAvailableCharacters_Implementation(const TArray<bool>& _AvailCharacters);
	virtual bool UpdateAvailableCharacters_Validate(const TArray<bool>& _AvailCharacters);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void AssignSelectedCharacter(int _CharID, UTexture2D* _CharImage);
	virtual void AssignSelectedCharacter_Implementation(int _CharID, UTexture2D* _CharImage);
	virtual bool AssignSelectedCharacter_Validate(int _CharID, UTexture2D* _CharImage);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Client, WithValidation)
		void AssignPlayer(TSubclassOf<class ACharacter> CharClass, UTexture2D* _CharImage);
	virtual void AssignPlayer_Implementation(TSubclassOf<class ACharacter> CharClass, UTexture2D* _CharImage);
	virtual bool AssignPlayer_Validate(TSubclassOf<class ACharacter> CharClass, UTexture2D* _CharImage);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void GetMyChatMessage(const FString& TextToSend);
	virtual void GetMyChatMessage_Implementation(const FString& TextToSend);
	virtual bool GetMyChatMessage_Validate(const FString& TextToSend);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Client, WithValidation)
		void UpdateMyChat(const FString& _SenderName, const FString& _SenderText);
	virtual void UpdateMyChat_Implementation(const FString& _SenderName, const FString& _SenderText);
	virtual bool UpdateMyChat_Validate(const FString& _SenderName, const FString& _SenderText);
};

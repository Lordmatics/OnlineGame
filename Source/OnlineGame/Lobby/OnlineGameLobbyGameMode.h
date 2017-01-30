// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MyStructs.h"
#include "OnlineGameLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API AOnlineGameLobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Transient)
		bool bCanWeStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		TArray<bool> bAvailableCharactersArray = { false,false,false,false,false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		TArray<APlayerController*> PlayerControllersArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		TArray<APlayerStart*> PlayerStartsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		TArray<TSubclassOf<ACharacter>> CharactersArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		TArray<FMyPlayerInfo> ConnectedPlayersArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		FString ServerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		FString GameModeMapName = FString(TEXT("Level_001"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		FString GameModeMapTime = FString(TEXT("10 : 00"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		UTexture2D* GameModeMapImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		int GameModeMapID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		int CurrentNumberOfPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		int MaxNumberOfPlayers;
public:

	FORCEINLINE TArray<APlayerController*> GetPlayerControllersArray() const { return PlayerControllersArray; }

	FORCEINLINE TArray<TSubclassOf<ACharacter>> GetCharactersArray() const { return CharactersArray; }

	FORCEINLINE TArray<bool> GetAvailableCharactersArray() const { return bAvailableCharactersArray; }

	FORCEINLINE void SetAvailableCharactersArrayElement(int _Index, bool _Condition) { bAvailableCharactersArray[_Index] = _Condition; }


	/** Called after a successful login.  This is the first place it is safe to call replicated functions on the PlayerAController. */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Called when a Controller with a PlayerState leaves the match. */
	virtual void Logout(AController* Exiting) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void RespawnPlayers(APlayerController* _PlayerController);
	virtual void RespawnPlayers_Implementation(APlayerController* _PlayerController);
	virtual bool RespawnPlayers_Validate(APlayerController* _PlayerController);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void EveryoneUpdate();
	virtual void EveryoneUpdate_Implementation();
	virtual bool EveryoneUpdate_Validate();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void ServerUpdateGameSettings(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime, int _MapID);
	virtual void ServerUpdateGameSettings_Implementation(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime, int _MapID);
	virtual bool ServerUpdateGameSettings_Validate(UTexture2D* _MapImage, const FString& _MapName, const FString& _MapTime, int _MapID);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")//, Reliable, Server, WithValidation)
		void LaunchTheGame();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void YouHaveBeenKicked(int _PlayerID);
	virtual void YouHaveBeenKicked_Implementation(int _PlayerID);
	virtual bool YouHaveBeenKicked_Validate(int _PlayerID);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")//, Reliable, Server, WithValidation)
		void AddToKickList();
	
	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void SwapCharacters(APlayerController* _PlayerController, TSubclassOf<class AOnlineGameCharacter> _CharacterClass, bool _bChangeStatus);
	virtual void SwapCharacters_Implementation(APlayerController* _PlayerController, TSubclassOf<class AOnlineGameCharacter> _CharacterClass, bool _bChangeStatus);
	virtual bool SwapCharacters_Validate(APlayerController* _PlayerController, TSubclassOf<class AOnlineGameCharacter> _CharacterClass, bool _bChangeStatus);
};

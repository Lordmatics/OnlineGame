// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "OnlineGameGameMode.generated.h"

UCLASS(minimalapi)//
class AOnlineGameGameMode : public AGameMode
{
	GENERATED_BODY()
private:

public:
	AOnlineGameGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++ Game Info")
		TArray<APlayerController*> PlayerControllers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++ Game Info")
		TArray<APlayerStart*> PlayerStarts;

public:

	/**
	* used to swap a viewport/connection's PlayerControllers when seamless traveling and the new GameMode's
	* controller class is different than the previous
	* includes network handling
	* @param OldPC - the old PC that should be discarded
	* @param NewPC - the new PC that should be used for the player
	*/
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) override;

	/** Called when a Controller with a PlayerState leaves the match. */
	virtual void Logout(AController* Exiting) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//UFUNCTION(BlueprintNativeEvent, Category = "C++ Game Info")
	//	void RespawnPlayer(APlayerController* _PlayerController, ACharacter* _PlayerCharacter);
	//virtual void RespawnPlayer_Implementation(APlayerController* _PlayerController, ACharacter* _PlayerCharacter);

//	UFUNCTION(reliable, server, WithValidation)
	//void ServerRespawnPlayerNetwork(APlayerController* _PlayerController, ACharacter* _PlayerCharacter);
	//virtual void RespawnPlayerNetwork_Implementation(APlayerController* _PlayerController, ACharacter* _PlayerCharacter);
	//virtual bool RespawnPlayerNetwork_Validate(APlayerController* _PlayerController, ACharacter* _PlayerCharacter);

};

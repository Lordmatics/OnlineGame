// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "OnlineGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API AOnlineGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AOnlineGamePlayerController();

	virtual void BeginPlay() override;
	
	//UFUNCTION(reliable, server, WithValidation)
	//void ServerPassCharacterInfoToPlayer();

	//virtual void PassCharacterInfoToPlayer_Implementation();
	//virtual bool PassCharacterInfoToPlayer_Validate();
	
private:

};

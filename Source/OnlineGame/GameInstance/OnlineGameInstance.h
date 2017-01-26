// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "OnlineGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API UOnlineGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:

public:

	void DestroySessionCaller(class AOnlineGamePlayerController* MyPlayerController);
	
	
};

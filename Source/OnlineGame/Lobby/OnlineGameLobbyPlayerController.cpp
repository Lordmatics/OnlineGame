// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "OnlineGameLobbyPlayerController.h"




void AOnlineGameLobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AOnlineGameGameMode, 5.0f);
}
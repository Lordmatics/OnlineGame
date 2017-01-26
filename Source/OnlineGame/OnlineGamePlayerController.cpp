// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "OnlineGamePlayerController.h"

AOnlineGamePlayerController::AOnlineGamePlayerController()
{

}

void AOnlineGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AOnlineGamePlayerController::PassCharacterInfoToPlayer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: PassCharacterInfoToPlayer Ran"));
}

bool AOnlineGamePlayerController::PassCharacterInfoToPlayer_Validate()
{
	return true;
}
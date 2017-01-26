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

//void AOnlineGamePlayerController::ServerPassCharacterInfoToPlayer()
//{
//	UE_LOG(LogTemp, Warning, TEXT("OnlineGamePlayerController: PassCharacterInfoToPlayer Ran"));
//}
//
//void AOnlineGamePlayerController::ServerPassCharacterInfoToPlayer_Implementation()
//{
//	ServerPassCharacterInfoToPlayer();
//}
//
//bool AOnlineGamePlayerController::ServerPassCharacterInfoToPlayer_Validate()
//{
//	return true;
//}
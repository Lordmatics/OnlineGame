// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "OnlineGameLobbyGameMode.h"




void AOnlineGameLobbyGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AOnlineGameGameMode, 5.0f);
}

// Initial Setup - Handles lobby setup on player controller
void AOnlineGameLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

// Removes player from lists, and updates arrays
void AOnlineGameLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

// Spawn player as base character when arriving into the lobby - Update all lobbies
void AOnlineGameLobbyGameMode::RespawnPlayers_Implementation()
{

}

bool AOnlineGameLobbyGameMode::RespawnPlayers_Validate()
{
	return true;
}

// Updates all Connected players lobby information when called
void AOnlineGameLobbyGameMode::EveryoneUpdate_Implementation()
{

}

bool AOnlineGameLobbyGameMode::EveryoneUpdate_Validate()
{
	return true;
}

// Server has made changes , tell each client to update their lobby information
void AOnlineGameLobbyGameMode::ServerUpdateGameSettings_Implementation()
{

}

bool AOnlineGameLobbyGameMode::ServerUpdateGameSettings_Validate()
{
	return true;
}

// Launch game with selected map
void AOnlineGameLobbyGameMode::LaunchTheGame()
{

}

// Removes player from lists, and updates arrays
void AOnlineGameLobbyGameMode::YouHaveBeenKicked_Implementation()
{

}

bool AOnlineGameLobbyGameMode::YouHaveBeenKicked_Validate()
{
	return true;
}

// Adds player to kick list, to handle the error, when kick window is open when someone new joins
// Auto refresh it
void AOnlineGameLobbyGameMode::AddToKickList()
{

}

// Change Character
void AOnlineGameLobbyGameMode::SwapCharacters_Implementation()
{

}

bool AOnlineGameLobbyGameMode::SwapCharacters_Validate()
{
	return true;
}
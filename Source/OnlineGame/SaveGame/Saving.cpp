// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "Saving.h"



void USaving::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AOnlineGameGameMode, 5.0f);
}

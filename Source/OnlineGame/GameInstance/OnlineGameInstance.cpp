// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "OnlineGameInstance.h"
#include "OnlineGamePlayerController.h"

void UOnlineGameInstance::DestroySessionCaller(AOnlineGamePlayerController* MyPlayerController)
{
	if (MyPlayerController != nullptr)
	{
		//APlayerController* PC = Cast<APlayerController>(MyPlayerController);
		//if (PC != nullptr)
		//{
		//	
		//}
	
		
		//MyPlayerController->DestroySession();
		
	}
}

void UOnlineGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AOnlineGameGameMode, 5.0f);
}
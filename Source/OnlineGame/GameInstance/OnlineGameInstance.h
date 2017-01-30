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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Transient)
		bool bSaveFileExists;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Transient)
		FString PlayerSaveString = FString(("PlayerSaveString"));
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		FString TheServerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		int MaxNumberOfPlayers = 0;
public:

	FORCEINLINE FString GetServerName() const { return TheServerName; }
	
	FORCEINLINE int GetMaxNumOfPlayers() const { return MaxNumberOfPlayers; }

	void DestroySessionCaller(class AOnlineGamePlayerController* MyPlayerController);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "Saving.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API USaving : public USaveGame
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++ Variables", meta = (AllowPrivateAccess = true))
		FMyPlayerInfo PlayerSettings;
public:

	FORCEINLINE FMyPlayerInfo GetPlayerStruct() const { return PlayerSettings; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};

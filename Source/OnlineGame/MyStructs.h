#pragma once

#include "Engine.h"
#include "MyStructs.generated.h"

USTRUCT()
struct FMyPlayerInfo
{
	//GENERATED_BODY()

		//Always make USTRUCT variables into UPROPERTY()
		//    any non-UPROPERTY() struct vars are not replicated
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FString PlayerName;

	UPROPERTY(EditAnywhere)
		FString PlayerLobbyStatus;

	UPROPERTY(EditAnywhere)
		UTexture2D* AvatarLogo;

	UPROPERTY(EditAnywhere)
		ACharacter* CharacterClass;

	UPROPERTY(EditAnywhere)
		UTexture2D* PlayerLogo;

	UPROPERTY(EditAnywhere)
		int32 PlayerLevel;

	UPROPERTY(EditAnywhere)
		float PlayerDamage;

	UPROPERTY(EditAnywhere)
		float PlayerFireRate;

	UPROPERTY(EditAnywhere)
		float PlayerHealth;

	UPROPERTY(EditAnywhere)
		float PlayerExperiencePoints;

	UPROPERTY(EditAnywhere)
		TMap<int32,float>  PlayerEXPTNL;

	//Constructor
	FMyPlayerInfo()
	{
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor
		PlayerName = FString(TEXT(""));
		PlayerLobbyStatus = FString(TEXT("Not Ready!"));
		AvatarLogo = nullptr;
		CharacterClass = nullptr;
		PlayerLogo = nullptr;
		PlayerLevel = 1;
		PlayerDamage = 10.0f;
		PlayerFireRate = 1.25f;
		PlayerHealth = 500.0f;
		PlayerExperiencePoints = 0.0f;
	}
};
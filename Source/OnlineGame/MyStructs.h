#pragma once

#include "Engine.h"
#include "MyStructs.generated.h"

USTRUCT(BlueprintType)
struct FMyPlayerInfo
{
	//GENERATED_BODY()

		//Always make USTRUCT variables into UPROPERTY()
		//    any non-UPROPERTY() struct vars are not replicated
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		FText PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText PlayerLobbyStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* ThePlayerProfileLogo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ACharacter> CharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* TheCharacterImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerExperiencePoints;

	//UPROPERTY(EditDefaultsOnly)
		//TMap<int, float>  PlayerEXPTNL;

	//Constructor
	FMyPlayerInfo()
	{
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor
		PlayerName = FText();
		PlayerLobbyStatus = FText();
		ThePlayerProfileLogo = nullptr; // Ideally this should be a random logo
		TheCharacterImage = nullptr;
		PlayerLevel = 1;
		PlayerDamage = 10.0f;
		PlayerFireRate = 1.25f;
		PlayerHealth = 500.0f;
		PlayerExperiencePoints = 0.0f;
		//PlayerEXPTNL.Add(0, 500.0f);
		//PlayerEXPTNL.Add(1, 1000.0f);
		//PlayerEXPTNL.Add(2, 2000.0f);
		//PlayerEXPTNL.Add(3, 3250.0f);
		//PlayerEXPTNL.Add(4, 4500.0f);
		//PlayerEXPTNL.Add(5, 6000.0f);
		//PlayerEXPTNL.Add(6, 7500.0f);
		//PlayerEXPTNL.Add(7, 9250.0f);
		//PlayerEXPTNL.Add(8, 11000.0f);
		//PlayerEXPTNL.Add(9, 13000.0f);
		//PlayerEXPTNL.Add(10, 15000.0f);
		//PlayerEXPTNL.Add(11, 17250.0f);
		//PlayerEXPTNL.Add(12, 19500.0f);
		//PlayerEXPTNL.Add(13, 22000.0f);
		//PlayerEXPTNL.Add(14, 24500.0f);
		//PlayerEXPTNL.Add(15, 27250.0f);
		//PlayerEXPTNL.Add(16, 30000.0f);
		//PlayerEXPTNL.Add(17, 33000.0f);
		//PlayerEXPTNL.Add(18, 36000.0f);
		//PlayerEXPTNL.Add(19, 39250.0f);
		//PlayerEXPTNL.Add(20, 42500.0f);
	}
};
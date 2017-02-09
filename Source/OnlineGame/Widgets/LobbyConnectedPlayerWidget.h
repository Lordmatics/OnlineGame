// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "LobbyConnectedPlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API ULobbyConnectedPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (ExposeOnSpawn = "true"))
		FMyPlayerInfo ThePlayerInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated)
		FText ThePlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated)
		UTexture2D* ThePlayersAvatar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated)
		UTexture2D* ThePlayersCharacterImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated)
		FText ThePlayerStatus;



protected:
	//virtual void NativeConstruct() override;
	//
	//
	//TSharedRef<SWidget> RebuildWidget() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

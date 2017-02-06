// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "LobbyPlayerButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API ULobbyPlayerButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = "true"))
		FString ThePlayerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = "true"))
		UTexture2D* ThePlayerImage;


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (ExposeOnSpawn = "true"))
		FMyPlayerInfo ThePlayerInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (ExposeOnSpawn = "true"))
		int ThePlayerIDToKick;

protected:
	virtual void NativeConstruct() override;
	
	TSharedRef<SWidget> RebuildWidget() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

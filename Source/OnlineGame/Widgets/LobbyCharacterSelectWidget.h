// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "LobbyCharacterSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API ULobbyCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
private:

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated)
		int CharSelectionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
		TArray<UTexture2D*> CharacterImagesArray;

protected:
	virtual void NativeConstruct() override;
	
	TSharedRef<SWidget> RebuildWidget() override;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

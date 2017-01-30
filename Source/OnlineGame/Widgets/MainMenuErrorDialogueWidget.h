// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "MainMenuErrorDialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API UMainMenuErrorDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

public:

protected:
	virtual void NativeConstruct() override;
	
	TSharedRef<SWidget> RebuildWidget() override;
};

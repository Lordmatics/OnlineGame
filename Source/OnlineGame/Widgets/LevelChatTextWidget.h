// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "LevelChatTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API ULevelChatTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
		int HelloWorld = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
	//	UTextBlock* TestTextBlock;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
	//	UButton* TestButton;

protected:
	virtual void NativeConstruct() override;

	TSharedRef<SWidget> RebuildWidget() override;

};

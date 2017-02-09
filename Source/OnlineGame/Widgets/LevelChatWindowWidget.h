// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "LevelChatWindowWidget.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API ULevelChatWindowWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
private:

public:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
	//	UBorder* Border;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
	//	UVerticalBox* VerticalBox;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
	//	UScrollBox* ScrollBox;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
	//	UEditableTextBox* TextBox;

//	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
//		void RebuildBorder(FString Name);
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
//		TSubclassOf<class ULevelChatTextWidget> ChatTextWidgetClass;
//
//	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Client, WithValidation)
//		void UpdateChatWindow(const FString& _Sender, const FString& _Text);
//	virtual void UpdateChatWindow_Implementation(const FString& _Sender, const FString& _Text);
//	virtual bool UpdateChatWindow_Validate(const FString& _Sender, const FString& _Text);
//
//
//	void TextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
//
//protected:
//	virtual void NativeConstruct() override;
//	
//	TSharedRef<SWidget> RebuildWidget() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "LobbyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API ULobbyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (AllowPrivateAccess = "true"))
		FText TheLobbyServerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (AllowPrivateAccess = "true"))
		FText TheReadyButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (AllowPrivateAccess = "true"))
		FText TheReadyStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = "true"))
		UButton* TheGameSettingsButton;
public:

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void EnableTheReadyStartButton();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated)
		FText TheMapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated)
		FText TheMapTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated)
		FText ThePlayersDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated)
		UTexture2D* TheMapImage;

protected:
	virtual void NativeConstruct() override;
	
	TSharedRef<SWidget> RebuildWidget() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};

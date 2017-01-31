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

	UPROPERTY(EditAnywhere, Category = "C++ Variables", meta = (AllowPrivateAccess = "true"))
		uint32 bFlipFlopIsReady : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ULobbyConnectedPlayerWidget> PlayerConnectedWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (AllowPrivateAccess = "true"))
		FText TheLobbyServerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (AllowPrivateAccess = "true"))
		FText TheReadyButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (AllowPrivateAccess = "true"))
		FText TheReadyStatus;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = "true"))
	//	UButton* TheGameSettingsButton;

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void CodeConstruct();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void ReadyButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void UpdateReadyStatus();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void ClearPlayerWindow();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void GameSettingsClicked();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void CharacterButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Client, WithValidation)
		void UpdatePlayerWindow(FMyPlayerInfo IncomingPlayerInfo);
	virtual void UpdatePlayerWindow_Implementation(FMyPlayerInfo IncomingPlayerInfo);
	virtual bool UpdatePlayerWindow_Validate(FMyPlayerInfo IncomingPlayerInfo);
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
		UButton* TheGameSettingsButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
		UVerticalBox* ThePlayerWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
		class ULobbyGameSettingsWidget* TheGameSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
		class ULobbyCharacterSelectWidget* TheCharacterSelect;

protected:
	virtual void NativeConstruct() override;
	
	TSharedRef<SWidget> RebuildWidget() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};

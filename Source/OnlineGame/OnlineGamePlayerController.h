// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "OnlineGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API AOnlineGamePlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Transient)
		FString PlayerSaveString = FString(("PlayerSaveString"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		FMyPlayerInfo PlayerSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		FText SenderText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", meta = (AllowPrivateAccess = true), Replicated)
		FText SenderName;
public:

	AOnlineGamePlayerController();

	// Loads Player Info and Pass over to Server
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void PassCharacterInfoToServer(FMyPlayerInfo PlayerSettingsInfo);
	virtual void PassCharacterInfoToServer_Implementation(FMyPlayerInfo PlayerSettingsInfo);
	virtual bool PassCharacterInfoToServer_Validate(FMyPlayerInfo PlayerSettingsInfo);
	

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void SetupMyChatWindow();
	virtual void SetupMyChatWindow_Implementation();
	virtual bool SetupMyChatWindow_Validate();

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void GetMyChatMessage(const FString& TextToSend);
	virtual void GetMyChatMessage_Implementation(const FString& TextToSend);
	virtual bool GetMyChatMessage_Validate(const FString& TextToSend);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions", Reliable, Server, WithValidation)
		void UpdateMyChat(const FString& _SenderName, const FString& _SenderText);
	virtual void UpdateMyChat_Implementation(const FString& _SenderName, const FString& _SenderText);
	virtual bool UpdateMyChat_Validate(const FString& _SenderName, const FString& _SenderText);

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void LoadGameViaSlot(const FString& SlotName, int32 UserIndex);

};

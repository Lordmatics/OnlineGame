// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "LobbyGameSettingsWidget.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEGAME_API ULobbyGameSettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (AllowPrivateAccess = "true"))
		int TheMapID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (AllowPrivateAccess = "true"))
		int TheTimeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (AllowPrivateAccess = "true"))
		FText DisplayMapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (AllowPrivateAccess = "true"))
		FText DisplayMapTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables", Replicated, meta = (AllowPrivateAccess = "true"))
		UTexture2D* DisplayMapImage;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
		UScrollBox* ChatScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ Variables")
		TSubclassOf<UUserWidget> PlayerButtonWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void FillThePlayersWindow();

protected:
	virtual void NativeConstruct() override;
	
	TSharedRef<SWidget> RebuildWidget() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

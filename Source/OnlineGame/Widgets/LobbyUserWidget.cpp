// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "LobbyUserWidget.h"
#include "Lobby/OnlineGameLobbyPlayerController.h"
#include "Lobby/OnlineGameLobbyGameMode.h"
#include "Widgets/LobbyConnectedPlayerWidget.h"
#include "Widgets/LobbyGameSettingsWidget.h"
#include "Widgets/LobbyCharacterSelectWidget.h"

//void ULobbyUserWidget::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	//AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(GetOwningPlayer());
//	//if (LobbyPC != nullptr)
//	//{
//	//	bool bIsServer = UKismetSystemLibrary::IsServer(this);
//	//	if (bIsServer)
//	//	{
//	//		TheReadyButtonText = FText::FromString(TEXT("Start Session!"));
//	//		// Host only needs to say host as opposed to the readying up
//	//		LobbyPC->GetPlayerSettings.PlayerState = TEXT("Host");
//	//	}
//	//	else
//	//	{
//	//		TheReadyButtonText = FText::FromString(TEXT("Toggle Ready!"));
//	//		// Clients shouldnt have access to settings
//	//		// Might just do this in the BP to be safe
//	//		if(TheGameSettingsButton != nullptr)
//	//			TheGameSettingsButton->RemoveFromParent();
//	//	}
//	//}
//}
//
//TSharedRef<SWidget> ULobbyUserWidget::RebuildWidget()
//{
//	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
//	//if (!RootWidget && WidgetTree)
//	//{
//	//	RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootWidget"));
//	//	UCanvasPanelSlot* RootWidgetSlot = Cast<UCanvasPanelSlot>(RootWidget->Slot);
//	//	if (RootWidgetSlot)
//	//	{
//	//		RootWidgetSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
//	//		RootWidgetSlot->SetOffsets(FMargin(0.0f, 0.0f));
//	//	}
//	//	WidgetTree->RootWidget = RootWidget;
//	//}
//	TSharedRef<SWidget> Widget = Super::RebuildWidget();
//
//	if (RootWidget && WidgetTree)
//	{
//		//TestTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("C++ Text Block"));
//		//RootWidget->AddChild(TestTextBlock);
//
//		//TestButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("C++ Button"));
//		//RootWidget->AddChild(TestButton);
//	}
//	//if (WidgetTree)
//	//{
//	//	// Root Widget is a UCanvasPanel
//	//	
//	//	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UPanelWidget>(UPanelWidget::StaticClass(), TEXT("RootWidget"));
//	//	// If GetRootWidget() is still null
//	//	WidgetTree->RootWidget = RootWidget;
//	//	if (RootWidget != nullptr && WidgetTree != nullptr)
//	//	{
//	//		TestTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("C++ Text Block")); // The second parameter is the name and is optional.
//	//		RootWidget->AddChild(TestTextBlock);
//	//	}
//	//}
//
//
//	return Widget;
//}
//
//void ULobbyUserWidget::EnableTheReadyStartButton()
//{
//
//}

void ULobbyUserWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULobbyUserWidget, TheLobbyServerName);

	DOREPLIFETIME(ULobbyUserWidget, TheReadyButtonText);

	DOREPLIFETIME(ULobbyUserWidget, TheReadyStatus);

	DOREPLIFETIME(ULobbyUserWidget, TheMapName);

	DOREPLIFETIME(ULobbyUserWidget, TheMapTime);

	DOREPLIFETIME(ULobbyUserWidget, ThePlayersDisplay);

	DOREPLIFETIME(ULobbyUserWidget, TheMapImage);



	
	//DOREPLIFETIME(AOnlineGameGameMode, 5.0f);
}

//// Called in BP Event Construct
//void ULobbyUserWidget::CodeConstruct()
//{
//	//// In BP Construct - Button and Vertical Box Initialised
//	//// And Game Settings 
//	//AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(GetOwningPlayer());
//	//if (LobbyPC != nullptr)
//	//{
//	//	bool bIsServer = UKismetSystemLibrary::IsServer(this);
//	//	if (bIsServer)
//	//	{
//	//		TheReadyButtonText = FText::FromString(TEXT("Start Session!"));
//	//		LobbyPC->GetPlayerSettings().PlayerLobbyStatus = TEXT("Host");
//	//	}
//	//	else
//	//	{
//	//		TheReadyButtonText = FText::FromString(TEXT("Toggle Ready!"));
//	//		// Initialise these values to prevent UI BUG
//	//		TheReadyStatus = FText::FromString(TEXT("Not Ready!"));
//	//		bFlipFlopIsReady = false;
//	//		// Clients dont want access to Game Settings
//	//		if(TheGameSettingsButton != nullptr)
//	//			TheGameSettingsButton->RemoveFromParent();
//	//	}
//	//}
//
//
//}
//
//void ULobbyUserWidget::ReadyButtonClicked()
//{
//	//UWorld* const World = GetWorld();
//	//if (World == nullptr) return;
//	//bool bIsServer = UKismetSystemLibrary::IsServer(this);
//	//if (bIsServer)
//	//{
//	//	AOnlineGameLobbyGameMode* LobbyGM = Cast<AOnlineGameLobbyGameMode>(World->GetAuthGameMode());
//	//	if (LobbyGM != nullptr)
//	//	{	
//	//		for (APlayerController* PC : LobbyGM->GetPlayerControllersArray())
//	//		{
//	//			AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(PC);
//	//			if (LobbyPC != nullptr)
//	//			{
//	//				// Push Loading Screen onto everyones clients since replicated
//	//				LobbyPC->ShowLoadingScreen();
//	//				// Server Travel from Host - Pushing all clients to chosen level
//	//				// NOTE: This might need doing outside the for loop, im not sure, since not replicated
//	//				LobbyGM->LaunchTheGame();
//	//			}
//	//		}
//	//	}
//	//}
//	//else
//	//{
//	//	UpdateReadyStatus();
//	//}
//}
//
//void ULobbyUserWidget::UpdateReadyStatus()
//{
//	//switch (bFlipFlopIsReady)
//	//{
//	//	case true:
//	//	{
//	//		TheReadyStatus = FText::FromString(TEXT("Ready!"));
//	//		break;
//	//	}
//	//	case false:
//	//	{
//	//		TheReadyStatus = FText::FromString(TEXT("Not Ready!"));
//	//		break;
//	//	}
//	//}
//	//AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(GetOwningPlayer());
//	//if (LobbyPC != nullptr)
//	//{
//	//	LobbyPC->GetPlayerSettings().PlayerLobbyStatus = TheReadyStatus.ToString();
//	//	LobbyPC->CallUpdate(LobbyPC->GetPlayerSettings(), false);
//	//	bFlipFlopIsReady = !bFlipFlopIsReady;
//	//}
//}
//
//void ULobbyUserWidget::ClearPlayerWindow()
//{
//	//if (ThePlayerWindow != nullptr)
//	//{
//	//	ThePlayerWindow->ClearChildren();
//	//}
//}
//
//void ULobbyUserWidget::UpdatePlayerWindow_Implementation(FMyPlayerInfo IncomingPlayerInfo)
//{
//	//ULobbyConnectedPlayerWidget* ConnectedPlayerWidget = CreateWidget<ULobbyConnectedPlayerWidget>(GetOwningPlayer(), PlayerConnectedWidgetClass);
//	//if (ConnectedPlayerWidget != nullptr)
//	//{
//	//	if (ThePlayerWindow != nullptr)
//	//	{
//	//		ThePlayerWindow->AddChild(ConnectedPlayerWidget);
//	//	}
//	//}
//	
//}
//
//bool ULobbyUserWidget::UpdatePlayerWindow_Validate(FMyPlayerInfo IncomingPlayerInfo)
//{
//	return true;
//}
//
//void ULobbyUserWidget::GameSettingsClicked()
//{
//	//if (TheGameSettings != nullptr)
//	//{
//	//	if (!TheGameSettings->IsVisible())
//	//	{
//	//		TheGameSettings->SetVisibility(ESlateVisibility::Visible);
//	//		TheGameSettings->FillThePlayersWindow();
//	//	}
//	//}
//}
//
//void ULobbyUserWidget::CharacterButtonClicked()
//{
//	//if (TheCharacterSelect != nullptr)
//	//{
//	//	if (!TheCharacterSelect->IsVisible())
//	//	{
//	//		TheCharacterSelect->bIsFocusable = true;
//	//		TheCharacterSelect->SetVisibility(ESlateVisibility::Visible);
//	//		TheCharacterSelect->SetUserFocus(GetOwningPlayer());
//	//	}
//	//}
//}
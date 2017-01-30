// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "LobbyUserWidget.h"
#include "Lobby/OnlineGameLobbyPlayerController.h"



void ULobbyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(GetOwningPlayer());
	//if (LobbyPC != nullptr)
	//{
	//	bool bIsServer = UKismetSystemLibrary::IsServer(this);
	//	if (bIsServer)
	//	{
	//		TheReadyButtonText = FText::FromString(TEXT("Start Session!"));
	//		// Host only needs to say host as opposed to the readying up
	//		LobbyPC->GetPlayerSettings.PlayerState = TEXT("Host");
	//	}
	//	else
	//	{
	//		TheReadyButtonText = FText::FromString(TEXT("Toggle Ready!"));
	//		// Clients shouldnt have access to settings
	//		// Might just do this in the BP to be safe
	//		if(TheGameSettingsButton != nullptr)
	//			TheGameSettingsButton->RemoveFromParent();
	//	}
	//}
}

TSharedRef<SWidget> ULobbyUserWidget::RebuildWidget()
{
	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
	//if (!RootWidget && WidgetTree)
	//{
	//	RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootWidget"));
	//	UCanvasPanelSlot* RootWidgetSlot = Cast<UCanvasPanelSlot>(RootWidget->Slot);
	//	if (RootWidgetSlot)
	//	{
	//		RootWidgetSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	//		RootWidgetSlot->SetOffsets(FMargin(0.0f, 0.0f));
	//	}
	//	WidgetTree->RootWidget = RootWidget;
	//}
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{
		//TestTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("C++ Text Block"));
		//RootWidget->AddChild(TestTextBlock);

		//TestButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("C++ Button"));
		//RootWidget->AddChild(TestButton);
	}
	//if (WidgetTree)
	//{
	//	// Root Widget is a UCanvasPanel
	//	
	//	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UPanelWidget>(UPanelWidget::StaticClass(), TEXT("RootWidget"));
	//	// If GetRootWidget() is still null
	//	WidgetTree->RootWidget = RootWidget;
	//	if (RootWidget != nullptr && WidgetTree != nullptr)
	//	{
	//		TestTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("C++ Text Block")); // The second parameter is the name and is optional.
	//		RootWidget->AddChild(TestTextBlock);
	//	}
	//}


	return Widget;
}

void ULobbyUserWidget::EnableTheReadyStartButton()
{

}

void ULobbyUserWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AOnlineGameGameMode, 5.0f);
}
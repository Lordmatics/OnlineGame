// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "MainMenuWidget.h"




//void UMainMenuWidget::NativeConstruct()
//{
//	Super::NativeConstruct();
//}
//
//TSharedRef<SWidget> UMainMenuWidget::RebuildWidget()
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
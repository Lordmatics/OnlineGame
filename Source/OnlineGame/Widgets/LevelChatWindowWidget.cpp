// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGame.h"
#include "LevelChatWindowWidget.h"
#include "Widgets/LevelChatTextWidget.h"
#include "Lobby/OnlineGameLobbyPlayerController.h"
#include "OnlineGamePlayerController.h"

void ULevelChatWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//RebuildWidget();
	//RebuildBorder(TEXT("C++ Border"));

	TextBox->OnTextCommitted.AddDynamic(this, &ULevelChatWindowWidget::TextCommitted);
}

void ULevelChatWindowWidget::RebuildBorder(FString VariableName)
{
	Border = (UBorder*)GetWidgetFromName(FName(*VariableName));
}

TSharedRef<SWidget> ULevelChatWindowWidget::RebuildWidget()
{
	//UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
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

	//if (RootWidget && WidgetTree)
	//{
	//	Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("C++ Border"));
	//	RootWidget->AddChild(Border);

	//	VerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("C++ Vertical Box"));
	//	Border->AddChild(VerticalBox);

	//	ScrollBox = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("C++ Scroll Box"));
	//	VerticalBox->AddChild(ScrollBox);

	//	TextBox = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("C++ Text Box"));
	//	VerticalBox->AddChild(TextBox);
		//TestTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("C++ Text Block"));
		//RootWidget->AddChild(TestTextBlock);

		//TestButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("C++ Button"));
		//RootWidget->AddChild(TestButton);
	//}
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

void ULevelChatWindowWidget::UpdateChatWindow_Implementation(const FString& _Sender, const FString& _Text)
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FText SenderName = FText::FromString(_Sender);
		FText SenderMessage = FText::FromString(_Text);
		FText Message;
		ULevelChatTextWidget* Chat = CreateWidget<ULevelChatTextWidget>(World, ChatTextWidgetClass);
		if (Chat != nullptr)
		{
			Chat->SetActualText(SenderMessage);
			if (ScrollBox != nullptr)
			{
				ScrollBox->AddChild(Chat);
				ScrollBox->ScrollToEnd();
			}
		}
	}
}

bool ULevelChatWindowWidget::UpdateChatWindow_Validate(const FString& _Sender, const FString& _Text)
{
	return true;
}

void ULevelChatWindowWidget::TextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		AOnlineGameLobbyPlayerController* LobbyPC = Cast<AOnlineGameLobbyPlayerController>(GetOwningPlayer());
		if (LobbyPC != nullptr)
		{
			LobbyPC->GetMyChatMessage(Text.ToString());
		}
		else
		{
			AOnlineGamePlayerController* GamePC = Cast<AOnlineGamePlayerController>(GetOwningPlayer());
			if (GamePC != nullptr)
			{
				GamePC->GetMyChatMessage(Text.ToString());
			}
		}
		if (TextBox != nullptr)
		{
			FString Empty = FString(TEXT(""));
			TextBox->SetText(FText::FromString(Empty));
		}
	}
}
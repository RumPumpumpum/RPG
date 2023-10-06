// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGQuestSlotWidget.h"
#include "Components/TextBlock.h"

void URPGQuestSlotWidget::SetQuestName(FString QuestName, int32 ID)
{
	QuestNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestNameText")));
	ensure(QuestNameText);

	QuestNameText->SetText(FText::FromString(QuestName));

	QuestID = ID;
}

void URPGQuestSlotWidget::ClickedQuestButton()
{
	OnQuestButtonClicked.Broadcast(QuestID);
}
 

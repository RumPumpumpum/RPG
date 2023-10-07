// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGQuestCountingWidget.h"

URPGQuestCountingWidget::URPGQuestCountingWidget()
{
	RPGGameInstance = Cast<URPGGameInstance>(GetGameInstance());
}

void URPGQuestCountingWidget::CreateCountingWidget(FString Name, int32 ID)
{
	QuestNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestNameText")));
	ensure(QuestNameText);

	QuestNameText->SetText(FText::FromString(Name));

	QuestID = ID;

}

void URPGQuestCountingWidget::UpdateQuestCount()
{
	KilledCntText = Cast<UTextBlock>(GetWidgetFromName(TEXT("KilledCntText")));
	ensure(KilledCntText);
	RequireCntText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RequireCntText")));
	ensure(RequireCntText);

	TArray<FRPGQuestData> QuestTable = RPGGameInstance->QuestTable;
	for (FRPGQuestData Quest : QuestTable)
	{
		if (Quest.QuestID == QuestID)
		{
			// 잡은 몬스터 수
			FString KilledCnt = FString::Printf(TEXT("%d"), Quest.KilledCnt);
			KilledCntText->SetText(FText::FromString(KilledCnt));

			// 잡아야할 몬스터 수
			FString RequireCnt = FString::Printf(TEXT("%d"), Quest.RequireCnt);
			RequireCntText->SetText(FText::FromString(RequireCnt));

			continue;
		}
	}
}

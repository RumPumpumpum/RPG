// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGHUDWidget.h"
#include "RPGHpBarWidget.h"
#include "RPGWidgetInterface.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "RPGQuestCountingWidget.h"


void URPGHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<URPGHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	IRPGWidgetInterface* WidgetInterface = Cast<IRPGWidgetInterface>(GetOwningPlayerPawn());
	if (WidgetInterface)
	{
		WidgetInterface->SetupWidget(this);
	}

	RPGGameInstance = Cast<URPGGameInstance>(GetGameInstance());
}

void URPGHUDWidget::UpdateHpBar(float NewCurrentHp, float CurrentMaxHp)
{
	HpBar->UpdateHpBar(NewCurrentHp, CurrentMaxHp);

	UTextBlock* CurrentHpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentHpText")));
	ensure(CurrentHpText);
	int32 CurrentMaxHpInt = FMath::FloorToInt(NewCurrentHp);
	FString CurrentHp = FString::Printf(TEXT("%d"), CurrentMaxHpInt);
	CurrentHpText->SetText(FText::FromString(CurrentHp));

	UTextBlock* MaxHpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxHpText")));
	ensure(MaxHpText);
	int32 MaxHpInt = FMath::FloorToInt(CurrentMaxHp);
	FString MaxHp = FString::Printf(TEXT("%d"), MaxHpInt);
	MaxHpText->SetText(FText::FromString(MaxHp));
}

void URPGHUDWidget::UpdateQuestCount()
{
	UVerticalBox* QuestVerticalBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("QuestVerticalBox")));
	if (QuestVerticalBox)
	{
		int32 NumChildren = QuestVerticalBox->GetChildrenCount();

		for (int32 Index = 0; Index < NumChildren; Index++)
		{
			URPGQuestCountingWidget* ChildWidget = Cast< URPGQuestCountingWidget>(QuestVerticalBox->GetChildAt(Index));

			if (ChildWidget)
			{
				ChildWidget->UpdateQuestCount();

			}
		}
	}
}

void URPGHUDWidget::SetQuestCountingWidgetClass(TSubclassOf<class URPGQuestCountingWidget> CountingWidgetClass)
{
	QuestCountingWidgetClass = CountingWidgetClass;
}

void URPGHUDWidget::UpdateQuestList()
{
	UVerticalBox* QuestVerticalBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("QuestVerticalBox")));
	ensure(QuestVerticalBox);

	// 기존 위젯 제거
	QuestVerticalBox->ClearChildren();

	TArray<FRPGQuestData> QuestTable = RPGGameInstance->QuestTable;
	for (FRPGQuestData Quest : QuestTable)
	{
		// 퀘스트가 완료 되었거나 수락하지 않았다면 건너뜀
		if (Quest.Completed || !Quest.Accepted)
		{
			continue;
		}

		QuestCountingWidget = CreateWidget<URPGQuestCountingWidget>(GetWorld(), QuestCountingWidgetClass);
		if(QuestCountingWidget)
		{
			QuestCountingWidget->CreateCountingWidget(Quest.QuestName, Quest.QuestID);
			QuestVerticalBox->AddChild(QuestCountingWidget);
		}
	}

	UpdateQuestCount();
}

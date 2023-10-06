// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGQuestMainWidget.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"


void URPGQuestMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	QuestSlotWidget = nullptr;

	QuestList = nullptr;

	RPGGameInstance = Cast<URPGGameInstance>(GetGameInstance());
}

void URPGQuestMainWidget::AddQuest(TSubclassOf<URPGQuestSlotWidget> QuestSlotWidgetClass)
{
	QuestList = Cast<UVerticalBox>(GetWidgetFromName(TEXT("QuestListVerticalBox")));

	TArray<FRPGQuestData> QuestTable = RPGGameInstance->QuestTable;
	for (FRPGQuestData Quest : QuestTable)
	{
		QuestSlotWidget = CreateWidget<URPGQuestSlotWidget>(GetWorld(), QuestSlotWidgetClass);
		QuestSlotWidget->SetQuestName(Quest.QuestName);
		QuestList->AddChild(QuestSlotWidget);
	}
}

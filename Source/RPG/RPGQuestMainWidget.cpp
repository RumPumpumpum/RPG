// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGQuestMainWidget.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "RPGBattleRewardInterface.h"


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
		// ����Ʈ�� �Ϸ� �Ǿ��ٸ� �ǳʶ�
		if (Quest.Completed)
		{
			continue;
		}

		QuestSlotWidget = CreateWidget<URPGQuestSlotWidget>(GetWorld(), QuestSlotWidgetClass);

		QuestSlotWidget->OnQuestButtonClicked.AddUObject(this, &URPGQuestMainWidget::SelectQuest);
		QuestSlotWidget->SetQuestName(Quest.QuestName, Quest.QuestID);
		QuestList->AddChild(QuestSlotWidget);
	}
}

void URPGQuestMainWidget::SelectQuest(int32 ID)
{
	SelectedQuestID = ID;

	UTextBlock* QuestNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestNameText")));
	ensure(QuestNameText);
	UTextBlock* DescriptionText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DescriptionText")));
	ensure(DescriptionText);
	UTextBlock* AcceptText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AcceptText")));
	ensure(AcceptText);
	UTextBlock* KilledCntText = Cast<UTextBlock>(GetWidgetFromName(TEXT("KilledCntText")));
	ensure(KilledCntText);
	UTextBlock* RequireCntText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RequireCntText")));
	ensure(RequireCntText);


	TArray<FRPGQuestData> QuestTable = RPGGameInstance->QuestTable;
	for (FRPGQuestData Quest : QuestTable)
	{
		if (Quest.QuestID == SelectedQuestID)
		{
			// ����Ʈ �̸�
			QuestNameText->SetText(FText::FromString(Quest.QuestName));

			// ����Ʈ ����
			DescriptionText->SetText(FText::FromString(Quest.Description));

			// ���� ���� ��
			FString KilledCnt = FString::Printf(TEXT("%d"), Quest.KilledCnt);
			KilledCntText->SetText(FText::FromString(KilledCnt));

			// ��ƾ��� ���� ��
			FString RequireCnt = FString::Printf(TEXT("%d"), Quest.RequireCnt);
			RequireCntText->SetText(FText::FromString(RequireCnt));

			// ���� ��ư
			if (!Quest.Accepted)
			{
				AcceptText->SetText(FText::FromString(TEXT("ACCEPT")));
			}
			else if (Quest.Accepted)
			{
				AcceptText->SetText(FText::FromString(TEXT("COMPLETE")));
			}

			break;
		}
	}
}

void URPGQuestMainWidget::AcceptQuest()
{
	TArray<FRPGQuestData>& QuestTable = RPGGameInstance->QuestTable;
	for (int32 i = 0; i < QuestTable.Num(); ++i)
	{
		if (QuestTable[i].QuestID == SelectedQuestID)
		{
			// �Ϸ�� ����Ʈ��� �ǳʶ�
			if (QuestTable[i].Completed)
			{
				continue;
			}

			// ���� ����Ʈ ������ ���ߴٸ�
			if (!QuestTable[i].Accepted)
			{
				QuestTable[i].Accepted = true;
				UTextBlock* AcceptText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AcceptText")));
				AcceptText->SetText(FText::FromString(TEXT("COMPLETE")));
			}

			// ����Ʈ�� ���� �� ���¶��
			else if (QuestTable[i].Accepted)
			{
				// ����Ʈ�� �Ϸ��� �� �ִٸ�
				if (QuestTable[i].KilledCnt >= QuestTable[i].RequireCnt)
				{
					QuestTable[i].Completed = true;
					UTextBlock* AcceptText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AcceptText")));
					AcceptText->SetText(FText::FromString(TEXT("CLEAR")));

					// ���� ����Ʈ ���� ����
					IRPGBattleRewardInterface* BattleRewardInterface = Cast<IRPGBattleRewardInterface>(GetOwningPlayerPawn());
					if(BattleRewardInterface)
					{
						BattleRewardInterface->StatPointReward(QuestTable[i].Reward);
					}
				}
			}

			break;
		}
	}
}

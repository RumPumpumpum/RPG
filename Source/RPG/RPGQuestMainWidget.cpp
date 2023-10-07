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
		// 퀘스트가 완료 되었다면 건너뜀
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
			// 퀘스트 이름
			QuestNameText->SetText(FText::FromString(Quest.QuestName));

			// 퀘스트 설명
			DescriptionText->SetText(FText::FromString(Quest.Description));

			// 잡은 몬스터 수
			FString KilledCnt = FString::Printf(TEXT("%d"), Quest.KilledCnt);
			KilledCntText->SetText(FText::FromString(KilledCnt));

			// 잡아야할 몬스터 수
			FString RequireCnt = FString::Printf(TEXT("%d"), Quest.RequireCnt);
			RequireCntText->SetText(FText::FromString(RequireCnt));

			// 수락 버튼
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
			// 완료된 퀘스트라면 건너뜀
			if (QuestTable[i].Completed)
			{
				continue;
			}

			// 아직 퀘스트 수락을 안했다면
			if (!QuestTable[i].Accepted)
			{
				QuestTable[i].Accepted = true;
				UTextBlock* AcceptText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AcceptText")));
				AcceptText->SetText(FText::FromString(TEXT("COMPLETE")));
			}

			// 퀘스트를 수락 한 상태라면
			else if (QuestTable[i].Accepted)
			{
				// 퀘스트를 완료할 수 있다면
				if (QuestTable[i].KilledCnt >= QuestTable[i].RequireCnt)
				{
					QuestTable[i].Completed = true;
					UTextBlock* AcceptText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AcceptText")));
					AcceptText->SetText(FText::FromString(TEXT("CLEAR")));

					// 스탯 포인트 보상 지급
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameInstance.h"

URPGGameInstance::URPGGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance"));
	// 퀘스트 리스트 설정
	static ConstructorHelpers::FObjectFinder<UDataTable> QuestListRef(TEXT("/Script/Engine.DataTable'/Game/QuestData/RPGQuestDataTable.RPGQuestDataTable'"));
	if (QuestListRef.Succeeded())
	{
		const UDataTable* QuestList = QuestListRef.Object;
		if (QuestList)
		{
			TArray<FName> RowNames = QuestList->GetRowNames();
			for (FName RowName : RowNames)
			{
				FRPGQuestData* QuestData = QuestList->FindRow<FRPGQuestData>(RowName, TEXT(""));
				if (QuestData)
				{
					QuestTable.Add(*QuestData);
				}
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGUserWidget.h"
#include "Components/TextBlock.h"
#include "RPGGameInstance.h"
#include "RPGQuestCountingWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGQuestCountingWidget : public URPGUserWidget
{
	GENERATED_BODY()

public:
	URPGQuestCountingWidget();

	void CreateCountingWidget(FString Name, int32 ID);

	void UpdateQuestCount();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UTextBlock> QuestNameText;

	UPROPERTY(BlueprintReadWrite, Category = "Widget")
	TObjectPtr<UTextBlock> KilledCntText;

	UPROPERTY(BlueprintReadWrite, Category = "Widget")
	TObjectPtr<UTextBlock> RequireCntText;

	UPROPERTY(BlueprintReadWrite, Category = "Quest")
	int32 QuestID;

private:
	URPGGameInstance* RPGGameInstance;

};

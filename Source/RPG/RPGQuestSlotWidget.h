// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGUserWidget.h"
#include "Components/TextBlock.h"
#include "RPGQuestSlotWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestButtonClicked, int32 /*QuestID*/);

UCLASS()
class RPG_API URPGQuestSlotWidget : public URPGUserWidget
{
	GENERATED_BODY()
	
public:
	void SetQuestName(FString QuestName, int32 ID);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void ClickedQuestButton();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UTextBlock> QuestNameText;

	UPROPERTY(BlueprintReadWrite, Category = "Quest")
	int32 QuestID;


public:
	FOnQuestButtonClicked OnQuestButtonClicked;
};

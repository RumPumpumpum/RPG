// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGUserWidget.h"
#include "Engine/DataTable.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "RPGQuestData.h"
#include "RPGGameInstance.h"
#include "RPGQuestSlotWidget.h"
#include "RPGQuestMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGQuestMainWidget : public URPGUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void AddQuest(TSubclassOf<URPGQuestSlotWidget> QuestSlotWidgetClass);

	void SelectQuest(int32 ID);

public:
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AcceptQuest();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<class URPGQuestSlotWidget> QuestSlotWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UVerticalBox> QuestList;

private:
	UPROPERTY()
	URPGGameInstance* RPGGameInstance;

	int32 SelectedQuestID;

};

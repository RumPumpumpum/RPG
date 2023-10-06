// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGUserWidget.h"
#include "Engine/DataTable.h"
#include "RPGQuestSlotWidget.h"
#include "Components/VerticalBox.h"
#include "RPGQuestData.h"
#include "RPGGameInstance.h"
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

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<URPGQuestSlotWidget> QuestSlotWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UVerticalBox> QuestList;

private:
	UPROPERTY()
	URPGGameInstance* RPGGameInstance; // URPGGameInstance 인스턴스에 대한 참조

};

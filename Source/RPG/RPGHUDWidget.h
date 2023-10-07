// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGUserWidget.h"
#include "RPGGameInstance.h"
#include "RPGHUDWidget.generated.h"


/**
 * 
 */
UCLASS()
class RPG_API URPGHUDWidget : public URPGUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateHpBar(float NewCurrentHp, float CurrentMaxHp);

	void UpdateQuestCount();

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void UpdateQuestList();

protected:
	UPROPERTY()
	TObjectPtr<class URPGHpBarWidget> HpBar;	

	UPROPERTY()
	TSubclassOf<class URPGQuestCountingWidget> QuestCountingWidgetClass;

	UPROPERTY()
	TObjectPtr<class URPGQuestCountingWidget> QuestCountingWidget;

public:
	void SetQuestCountingWidgetClass(TSubclassOf<class URPGQuestCountingWidget> CountingWidgetClass);

private:
	URPGGameInstance* RPGGameInstance;
};

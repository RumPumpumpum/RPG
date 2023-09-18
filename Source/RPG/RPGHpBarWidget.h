// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGUserWidget.h"
#include "RPGHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGHpBarWidget : public URPGUserWidget
{
	GENERATED_BODY()

public:
	URPGHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateHpBar(float NewCurrentHp);

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpBar;

	UPROPERTY()
	float MaxHp;
};

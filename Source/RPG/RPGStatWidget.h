// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGUserWidget.h"
#include "RPGStatWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxHpChangedDelegate, float /*MaxHp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageChangedDelegate, float /*Damage*/);

UCLASS()
class RPG_API URPGStatWidget : public URPGUserWidget
{
	GENERATED_BODY()

public:
	URPGStatWidget();

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateMaxHp(float NewMaxHp);

	void UpdateDamage(float NewDamage);

	void UpdateStatPoint(int NewStatPoint);

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> MaxHpText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> DamageText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> StatPointText;

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float Damage;
	
	UPROPERTY()
	int StatPoint;

public:
	UFUNCTION(BlueprintCallable, Category = "Stat")
	void IncreaseMaxHp(float Value);

	UFUNCTION(BlueprintCallable, Category = "Stat")
	void IncreaseDamage(float Value);

	UFUNCTION(BlueprintCallable, Category = "Stat")
	void IncreaseStatPoint(int ChangeAmount);


public:
	// DELEGATE
	FOnMaxHpChangedDelegate OnMaxHpChanged;
	FOnDamageChangedDelegate OnDamageChanged;
};
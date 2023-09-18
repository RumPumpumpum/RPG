// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API URPGCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	// Transient : 현재 HP 같은 경우 게임을 할 때 마다 새롭게 지정되기 때문에
	// 디스크에 저장할 필요가 없을 수도 있음
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

public:

	void ApplyDamege(float InDamage);
	void SetHp(float NewHp);

public:
	// DELEGATE
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	// FORCEINLINE
	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
};

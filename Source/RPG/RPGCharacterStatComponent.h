// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float /*CurrentHp*/, float /*MaxHp*/);

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

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackDamage;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float DefenseRange;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int StatPoint;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float RespawnTime;

public:
	void ApplyDamage(float InDamage);
	void SetHp(float NewHp);
	void SetMaxHp(float NewMaxHp);
	void SetDamage(float NewDamage);
	void SetStatPoint(int ChangeAmount);

public:
	// DELEGATE
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	// FORCEINLINE
	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetAttackRange() { return AttackRange; }
	FORCEINLINE float GetAttackDamage() { return AttackDamage; }
	FORCEINLINE float GetDefenseRange() { return DefenseRange; }
	FORCEINLINE float GetStatPoint() { return StatPoint; }
	FORCEINLINE float GetRespawnTime() { return RespawnTime; }
};

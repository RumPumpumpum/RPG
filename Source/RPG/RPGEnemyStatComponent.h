// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGEnemyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float /*CurrentHp*/, float /*MaxHp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API URPGEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGEnemyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float PatrolRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float DetectRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float DefenseRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float RespawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float StunTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int RewardHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int RewardStatPoint;

public:
	void ApplyDamage(float InDamage);
	void SetHp(float NewHp);

public:
	// DELEGATE
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	// FORCEINLINE
	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetPatrolRadius() { return PatrolRadius; }
	FORCEINLINE float GetDetectRadius() { return DetectRadius; }
	FORCEINLINE float GetAttackRange() { return AttackRange; }
	FORCEINLINE float GetAttackDamage() { return AttackDamage; }
	FORCEINLINE float GetDefenseRange() { return DefenseRange; }
	FORCEINLINE float GetRespawnTime() { return RespawnTime; }
	FORCEINLINE float GetStunTime() { return StunTime; }
	FORCEINLINE float GetRewardHP() { return RewardHP; }
	FORCEINLINE int GetRewardStatPoint() { return RewardStatPoint; }
};

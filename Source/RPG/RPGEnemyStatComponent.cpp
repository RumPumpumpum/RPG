// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemyStatComponent.h"

// Sets default values for this component's properties
URPGEnemyStatComponent::URPGEnemyStatComponent()
{
	// 체력 설정
	MaxHp = 200.f;
	SetHp(MaxHp);

	// AI 범위 설정
	PatrolRadius = 800.0f;
	DetectRadius = 700.0f;

	// 공격 설정
	AttackRange = 200.0f;
	AttackDamage = 30.f;
	
	// 방어 설정
	DefenseRange = 100.0f;

	// 리스폰 시간 설정
	RespawnTime = 8.0f;
}


// Called when the game starts
void URPGEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URPGEnemyStatComponent::ApplyDamage(float InDamage)
{
	SetHp(CurrentHp - InDamage);
}

void URPGEnemyStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp, 0.0f, MaxHp);

	if (CurrentHp <= 0.0f)
	{
		OnHpZero.Broadcast();
	}

	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}



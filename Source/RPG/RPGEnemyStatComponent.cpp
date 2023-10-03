// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemyStatComponent.h"

// Sets default values for this component's properties
URPGEnemyStatComponent::URPGEnemyStatComponent()
{
	// ü�� ����
	MaxHp = 200.f;
	SetHp(MaxHp);

	// AI ���� ����
	PatrolRadius = 800.0f;
	DetectRadius = 700.0f;

	// ���� ����
	AttackRange = 200.0f;
	AttackDamage = 30.f;
	
	// ��� ����
	DefenseRange = 100.0f;

	// ������ �ð� ����
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



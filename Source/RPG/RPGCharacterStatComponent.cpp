// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacterStatComponent.h"

// Sets default values for this component's properties
URPGCharacterStatComponent::URPGCharacterStatComponent()
{
	// ü�� ����
	MaxHp = 100.f;
	SetHp(MaxHp);

	// ���� ����
	AttackRange = 150.0f;
	AttackDamage = 30.0f;

	// ��� ����
	DefenseRange = 100.0f;

	// ���� ����Ʈ ����
	StatPoint = 5;

	// ������ �ð�
	RespawnTime = 5.0f;
}


// Called when the game starts
void URPGCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void URPGCharacterStatComponent::ApplyDamage(float InDamage)
{
	SetHp(CurrentHp - InDamage);
}

void URPGCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp, 0.0f, MaxHp);

	if (CurrentHp <= 0.0f)
	{
		OnHpZero.Broadcast();
	}

	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void URPGCharacterStatComponent::SetMaxHp(float NewMaxHp)
{
	float IncreaseHp = NewMaxHp - MaxHp;
	MaxHp = NewMaxHp;

	SetHp(CurrentHp + IncreaseHp);
}

void URPGCharacterStatComponent::SetDamage(float NewDamage)
{
	AttackDamage = NewDamage;
}

void URPGCharacterStatComponent::SetStatPoint(int ChangeAmount)
{
	StatPoint += ChangeAmount;
}



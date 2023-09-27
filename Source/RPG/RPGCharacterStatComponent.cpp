// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacterStatComponent.h"

// Sets default values for this component's properties
URPGCharacterStatComponent::URPGCharacterStatComponent()
{
	// 체력 설정
	MaxHp = 100.f;
	SetHp(MaxHp);

	// 공격 설정
	AttackRange = 150.0f;
	AttackDamage = 30.0f;

	// 방어 설정
	DefenseRange = 100.0f;

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

	OnHpChanged.Broadcast(CurrentHp);
}



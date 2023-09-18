// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacterStatComponent.h"

// Sets default values for this component's properties
URPGCharacterStatComponent::URPGCharacterStatComponent()
{
	MaxHp = 100.f;
	SetHp(MaxHp);
}


// Called when the game starts
void URPGCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URPGCharacterStatComponent::ApplyDamege(float InDamage)
{
	const float ActualDamage = InDamage;
	const float PrevHp = CurrentHp;

	SetHp(PrevHp - ActualDamage);
}

void URPGCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp, 0.0f, MaxHp);

	if (CurrentHp <= 0.0f)
	{
		OnHpZero.Broadcast();
	}

	// HP가 변경되었으니 나를 구독한 사람은 알아서 처리하도록!
	OnHpChanged.Broadcast(CurrentHp);
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemyStatComponent.h"

// Sets default values for this component's properties
URPGEnemyStatComponent::URPGEnemyStatComponent()
{
	MaxHp = 200.f;
	SetHp(MaxHp);
}


// Called when the game starts
void URPGEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URPGEnemyStatComponent::ApplyDamege(float InDamage)
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

	OnHpChanged.Broadcast(CurrentHp);
}



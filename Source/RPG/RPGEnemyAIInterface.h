// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPGEnemyAIInterface.generated.h"

// ������ �������� �˸��� ���� ��������Ʈ
DECLARE_DELEGATE(FAIAttackFinished)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URPGEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class RPG_API IRPGEnemyAIInterface
{
	GENERATED_BODY()

// ���� ����
public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRadius() = 0;
	virtual float GetAIAttackRange() = 0;

// ���� ����
public:
	virtual void SetAIAttackFinishedDelegate(const FAIAttackFinished& AIAttackFinished) = 0;
	virtual void StartAIAttack() = 0;
};

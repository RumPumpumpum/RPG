// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPGAnimationAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URPGAnimationAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API IRPGAnimationAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//순수 가상함수, 하위 클래스에서 구현을 강제
	virtual void AttackHitCheck() = 0;
};

	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RPGAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ARPGAIController();

	void StartAI();
	void StopAI();

protected:
	// � ��Ʈ�ѷ��� ���� �����ؼ� ������ �� �߻��Ǵ� �̺�Ʈ�Լ�
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<UBlackboardData> BBData;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BTAsset;
};

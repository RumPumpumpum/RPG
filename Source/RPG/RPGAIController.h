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
	// 어떤 컨트롤러가 폰에 빙의해서 조종할 때 발생되는 이벤트함수
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<UBlackboardData> BBData;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BTAsset;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "RPGEnemyAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Super 클래스의 함수 성공 여부를 저장
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 현재 컨트롤중인 Pawn을 가져오는데 실패했으면 Failed 반환
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!CurrentPawn)
	{
		return EBTNodeResult::Failed;
	}

	// AI 인터페이스
	IRPGEnemyAIInterface* AICurrentPawn = Cast<IRPGEnemyAIInterface>(CurrentPawn);
	float PatrolRadius = AICurrentPawn->GetAIPatrolRadius();
	if (!AICurrentPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAIAttackFinished AIAttackFinished;
	AIAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AICurrentPawn->SetAIAttackFinishedDelegate(AIAttackFinished);
	AICurrentPawn->StartAIAttack();

	return EBTNodeResult::InProgress;
}



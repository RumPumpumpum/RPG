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
	// Super Ŭ������ �Լ� ���� ���θ� ����
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// ���� ��Ʈ������ Pawn�� �������µ� ���������� Failed ��ȯ
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!CurrentPawn)
	{
		return EBTNodeResult::Failed;
	}

	// AI �������̽�
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



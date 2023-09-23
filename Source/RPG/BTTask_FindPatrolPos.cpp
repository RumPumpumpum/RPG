// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RPGEnemyAIInterface.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Super Ŭ������ �Լ� ���� ���θ� ����
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// ���� ��Ʈ������ Pawn�� �������µ� ���������� Failed ��ȯ
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!CurrentPawn)
	{
		return EBTNodeResult::Failed;
	}

	// �׺���̼� �ý����� �������µ� ���������� Failed ��ȯ 
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (!NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	// AI�� ������ ���õ� �������̽�
	IRPGEnemyAIInterface* AICurrentPawn = Cast<IRPGEnemyAIInterface>(CurrentPawn);
	float PatrolRadius = AICurrentPawn->GetAIPatrolRadius();
	if (!AICurrentPawn)
	{
		return EBTNodeResult::Failed;
	}

	// ������� ���� CurrentPos�� �޾ƿ� ����
	FVector CurrentPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("CurrentPos")));
	FNavLocation PatrolPos;
	
	// CurrentPos ��ġ�κ��� PatrolRadius �̳��� ���������� ������ PatrolPos�� ����
	if (NavSystem->GetRandomPointInNavigableRadius(CurrentPos, PatrolRadius, PatrolPos))
	{
		//�������� PatrolPos�� �������� ��ġ�� ����
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PatrolPos")), PatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

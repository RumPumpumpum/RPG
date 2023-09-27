// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "AIController.h"
#include "RPGEnemyAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("TurnToTarget");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Super Ŭ������ �Լ� ���� ���θ� ����
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// ���� ��Ʈ������ Pawn�� �������µ� ���������� Failed ��ȯ
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!CurrentPawn)
	{
		return EBTNodeResult::Failed;
	}

	// Target�� �������µ� ���������� Failed ��ȯ
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
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

	FVector TargetLocation = Target->GetActorLocation();
	FVector DirectionToTarget = TargetLocation - CurrentPawn->GetActorLocation();
	FRotator RotatorToTarget = DirectionToTarget.Rotation();

	// ���Ʒ� ȸ�� ����
	RotatorToTarget.Pitch = 0.0f;

	CurrentPawn->SetActorRotation(RotatorToTarget);

	return EBTNodeResult::Succeeded;
}

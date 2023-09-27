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
	// Super 클래스의 함수 성공 여부를 저장
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 현재 컨트롤중인 Pawn을 가져오는데 실패했으면 Failed 반환
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!CurrentPawn)
	{
		return EBTNodeResult::Failed;
	}

	// Target을 가져오는데 실패했으면 Failed 반환
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
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

	FVector TargetLocation = Target->GetActorLocation();
	FVector DirectionToTarget = TargetLocation - CurrentPawn->GetActorLocation();
	FRotator RotatorToTarget = DirectionToTarget.Rotation();

	// 위아래 회전 방지
	RotatorToTarget.Pitch = 0.0f;

	CurrentPawn->SetActorRotation(RotatorToTarget);

	return EBTNodeResult::Succeeded;
}

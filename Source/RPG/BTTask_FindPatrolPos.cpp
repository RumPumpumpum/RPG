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
	// Super 클래스의 함수 성공 여부를 저장
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 현재 컨트롤중인 Pawn을 가져오는데 실패했으면 Failed 반환
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!CurrentPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 네비게이션 시스템을 가져오는데 실패했으면 Failed 반환 
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (!NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	// AI의 범위에 관련된 인터페이스
	IRPGEnemyAIInterface* AICurrentPawn = Cast<IRPGEnemyAIInterface>(CurrentPawn);
	float PatrolRadius = AICurrentPawn->GetAIPatrolRadius();
	if (!AICurrentPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 블랙보드로 부터 CurrentPos를 받아와 저장
	FVector CurrentPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("CurrentPos")));
	FNavLocation PatrolPos;
	
	// CurrentPos 위치로부터 PatrolRadius 이내에 랜덤범위를 지정해 PatrolPos에 저장
	if (NavSystem->GetRandomPointInNavigableRadius(CurrentPos, PatrolRadius, PatrolPos))
	{
		//블랙보드의 PatrolPos에 랜덤범위 위치를 저장
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PatrolPos")), PatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

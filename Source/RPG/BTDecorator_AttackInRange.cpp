// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RPGEnemyAIInterface.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// ���� �����ͼ� ���� ��ġ�� ����
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!CurrentPawn)
	{
		return EBTNodeResult::Failed;
	}
	FVector Center = CurrentPawn->GetActorLocation();

	// ���� Ÿ���� �ִ��� üũ
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	// AIInterface�� ����ȯ�Ͽ� AttackRange�� ����
	IRPGEnemyAIInterface* CurrentAIPawn = Cast<IRPGEnemyAIInterface>(CurrentPawn);
	if (!CurrentAIPawn)
	{
		return EBTNodeResult::Failed;
	}

	float AttackRange = CurrentAIPawn->GetAIAttackRange();
	float TargetRange = CurrentPawn->GetDistanceTo(Target);
	
	bResult = (TargetRange <= AttackRange);

	return bResult;
}

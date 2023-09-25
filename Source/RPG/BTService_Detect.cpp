// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "RPGEnemyAIInterface.h"
#include "RPGAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// ���� �����ͼ� ���� ��ġ�� ����
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!CurrentPawn)
	{
		return;
	}
	FVector Center = CurrentPawn->GetActorLocation();

	// AIInterface�� ����ȯ�Ͽ� DectectRange�� ����
	IRPGEnemyAIInterface* CurrentAIPawn = Cast<IRPGEnemyAIInterface>(CurrentPawn);
	if (!CurrentAIPawn)
	{
		return;
	}
	float DetectRange = CurrentAIPawn->GetAIDetectRadius();

	// �浹�˻�
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Detect), false, CurrentPawn);

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(DetectRange),
		Params);
	if (bResult)
	{
		// ������ ��� Pawn�� ���ؼ� �˻縦 ����
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());

			// Ž���� Pawn�� PlayerContoller�� �����ϰ� �ִ� ���
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Pawn);
				// !�浹���� �׽�Ʈ!
				DrawDebugSphere(GetWorld(), Center, DetectRange, 16, FColor::Green, false, 0.2f);
				return;
			}
		}
	}

	// Pawn�� �������� ������ ���
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);

	// !�浹���� �׽�Ʈ!
	DrawDebugSphere(GetWorld(), Center, DetectRange, 16, FColor::Red, false, 0.2f);
}
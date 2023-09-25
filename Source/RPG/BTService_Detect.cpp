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

	// 폰을 가져와서 현재 위치를 저장
	APawn* CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!CurrentPawn)
	{
		return;
	}
	FVector Center = CurrentPawn->GetActorLocation();

	// AIInterface로 형변환하여 DectectRange를 설정
	IRPGEnemyAIInterface* CurrentAIPawn = Cast<IRPGEnemyAIInterface>(CurrentPawn);
	if (!CurrentAIPawn)
	{
		return;
	}
	float DetectRange = CurrentAIPawn->GetAIDetectRadius();

	// 충돌검사
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
		// 감지된 모든 Pawn에 대해서 검사를 수행
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());

			// 탐지된 Pawn을 PlayerContoller가 조종하고 있는 경우
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Pawn);
				// !충돌판정 테스트!
				DrawDebugSphere(GetWorld(), Center, DetectRange, 16, FColor::Green, false, 0.2f);
				return;
			}
		}
	}

	// Pawn을 감지하지 못했을 경우
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);

	// !충돌판정 테스트!
	DrawDebugSphere(GetWorld(), Center, DetectRange, 16, FColor::Red, false, 0.2f);
}
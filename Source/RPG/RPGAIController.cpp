// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ARPGAIController::ARPGAIController()
{
	// BlackBoard
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBDataRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_RPGEnemy.BB_RPGEnemy'"));
	if (BBDataRef.Succeeded())
	{
		BBData = BBDataRef.Object;
	}

	// Behavior Tree
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_RPGEnemy.BT_RPGEnemy'"));
	if (BTAssetRef.Succeeded())
	{
		BTAsset = BTAssetRef.Object;
	}
}

void ARPGAIController::StartAI()
{
	// 블랙보드 컴포넌트 설정
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BBData, BlackboardComp))
	{
		if (RunBehaviorTree(BTAsset))
		{
			// 블랙보드의 CurrentPos에 현재 컨트롤중인 폰의 위치를 저장
			Blackboard->SetValueAsVector(FName(TEXT("CurrentPos")), GetPawn()->GetActorLocation());
		}
	}
}

void ARPGAIController::StopAI()
{
	// 행동트리를 썼기 때문에 BT로 캐스팅 가능, 혹시 다른 기능 썼으면 불가
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp)
	{
		BTComp->StopTree();
	}
}

// AI컨트롤러가 AI캐릭터를 조종하기 시작할 때 호출
void ARPGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	StartAI();
}

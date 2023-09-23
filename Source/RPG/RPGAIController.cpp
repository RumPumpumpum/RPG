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
	// ������ ������Ʈ ����
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BBData, BlackboardComp))
	{
		if (RunBehaviorTree(BTAsset))
		{
			// �������� CurrentPos�� ���� ��Ʈ������ ���� ��ġ�� ����
			Blackboard->SetValueAsVector(FName(TEXT("CurrentPos")), GetPawn()->GetActorLocation());
		}
	}
}

void ARPGAIController::StopAI()
{
	// �ൿƮ���� ��� ������ BT�� ĳ���� ����, Ȥ�� �ٸ� ��� ������ �Ұ�
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp)
	{
		BTComp->StopTree();
	}
}

// AI��Ʈ�ѷ��� AIĳ���͸� �����ϱ� ������ �� ȣ��
void ARPGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	StartAI();
}

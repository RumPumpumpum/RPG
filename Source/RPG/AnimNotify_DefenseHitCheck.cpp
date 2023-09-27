// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DefenseHitCheck.h"
#include "RPGAnimationDefenseInterface.h"

void UAnimNotify_DefenseHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IRPGAnimationDefenseInterface* DefensePawn = Cast<IRPGAnimationDefenseInterface>(MeshComp->GetOwner());
		if (DefensePawn)
		{
			DefensePawn->DefenseHitCheck();
		}
	}
}

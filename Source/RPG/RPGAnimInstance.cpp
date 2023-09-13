// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

URPGAnimInstance::URPGAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Character/AM_Attack.AM_Attack'"));
	if (AttackMontageRef.Succeeded())
	{
		AttackMontage = AttackMontageRef.Object;
	}
}

void URPGAnimInstance::AnimNotify_Land()
{
	Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void URPGAnimInstance::AnimNotify_LandFinish()
{
	Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void URPGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void URPGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		Speed = Velocity.Size2D();
		bIsIdle = Speed <= 0.f;
		bIsInAir = Movement->IsFalling();
	}
}

void URPGAnimInstance::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (!Montage_IsPlaying(AttackMontage))
	{
		AnimInstance->Montage_Play(AttackMontage);
	}
}

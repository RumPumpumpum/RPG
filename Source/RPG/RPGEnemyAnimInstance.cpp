// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URPGEnemyAnimInstance::URPGEnemyAnimInstance()
{
	// °ø°Ý ¸ùÅ¸ÁÖ ·Îµå
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Enemy/Animations/AM_EnemyAttack.AM_EnemyAttack'"));
	if (AttackMontageRef.Succeeded())
	{
		AttackMontage = AttackMontageRef.Object;
	}

	// »ç¸Á ¸ùÅ¸ÁÖ ·Îµå
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Enemy/Animations/AM_EnemyDead.AM_EnemyDead'"));
	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}
}

void URPGEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void URPGEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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

void URPGEnemyAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void URPGEnemyAnimInstance::AnimNotify_AttackStart()
{
	bIsAttacking = true;
}

void URPGEnemyAnimInstance::AnimNotify_AttackFinish()
{
	bIsAttacking = false;
}

void URPGEnemyAnimInstance::PlayDeadMontage()
{
	StopAllMontages(0.f);
	Montage_Play(DeadMontage);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


URPGEnemyAnimInstance::URPGEnemyAnimInstance()
{
	// 공격 몽타주 로드
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Enemy/Animations/AM_EnemyAttack.AM_EnemyAttack'"));
	if (AttackMontageRef.Succeeded())
	{
		AttackMontage = AttackMontageRef.Object;
	}

	// 사망 몽타주 로드
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Enemy/Animations/AM_EnemyDead.AM_EnemyDead'"));
	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// 상태 초기화
	Stunned = false;

	// 사운드 큐 설정
	static ConstructorHelpers::FObjectFinder<USoundCue> AttackSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Enemy_Attack.Enemy_Attack'"));
	if (AttackSoundCueRef.Succeeded())
	{
		AttackSoundCue = AttackSoundCueRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SwordSmashSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Sword_Smash.Sword_Smash'"));
	if (SwordSmashSoundCueRef.Succeeded())
	{
		SwordSmashSoundCue = SwordSmashSoundCueRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> DeadSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Dead.Dead'"));
	if (DeadSoundCueRef.Succeeded())
	{
		DeadSoundCue = DeadSoundCueRef.Object;
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
	UGameplayStatics::PlaySound2D(this, AttackSoundCue);
	UGameplayStatics::PlaySound2D(this, SwordSmashSoundCue);
}

void URPGEnemyAnimInstance::AnimNotify_AttackFinish()
{
	bIsAttacking = false;
}

void URPGEnemyAnimInstance::PlayDeadMontage()
{
	StopAllMontages(0.f);
	Montage_Play(DeadMontage);
	UGameplayStatics::PlaySound2D(this, DeadSoundCue);

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


URPGAnimInstance::URPGAnimInstance()
{
	// ���� ��Ÿ�� �ε�
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Character/Animations/AM_Attack.AM_Attack'"));
	if (AttackMontageRef.Succeeded())
	{
		AttackMontage = AttackMontageRef.Object;
	}

	// ��� ��Ÿ�� �ε�
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Character/Animations/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// ��� ��Ÿ�� �ε�
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DefenseMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Character/Animations/AM_Defense.AM_Defense'"));
	if (DefenseMontageRef.Succeeded())
	{
		DefenseMontage = DefenseMontageRef.Object;
	}

	// ���� ť ����
	static ConstructorHelpers::FObjectFinder<USoundCue> AttackSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Character_Attack.Character_Attack'"));
	if (AttackSoundCueRef.Succeeded())
	{
		AttackSoundCue = AttackSoundCueRef.Object;
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
	Montage_Play(AttackMontage);
	UGameplayStatics::PlaySound2D(this, AttackSoundCue);
}

void URPGAnimInstance::JumpToSectionAttackMontage(uint8 AttackCnt)
{
	FName NextAttackSection = *FString::Printf(TEXT("Attack%d"), AttackCnt);
	Montage_JumpToSection(NextAttackSection, AttackMontage);
}

void URPGAnimInstance::PlayDefenseMontage()
{
	Montage_Play(DefenseMontage);
}

void URPGAnimInstance::PlayDeadMontage()
{
	StopAllMontages(0.f);
	Montage_Play(DeadMontage);
}

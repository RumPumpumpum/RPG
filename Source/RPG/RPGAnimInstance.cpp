// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


URPGAnimInstance::URPGAnimInstance()
{
	// 공격 몽타주 로드
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Character/Animations/AM_Attack.AM_Attack'"));
	if (AttackMontageRef.Succeeded())
	{
		AttackMontage = AttackMontageRef.Object;
	}

	// 사망 몽타주 로드
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Character/Animations/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// 방어 몽타주 로드
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DefenseMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Character/Animations/AM_Defense.AM_Defense'"));
	if (DefenseMontageRef.Succeeded())
	{
		DefenseMontage = DefenseMontageRef.Object;
	}

	// 사운드 큐 설정
	static ConstructorHelpers::FObjectFinder<USoundCue> AttackSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Character_Attack.Character_Attack'"));
	if (AttackSoundCueRef.Succeeded())
	{
		AttackSoundCue = AttackSoundCueRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SwordSmashSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Sword_Smash.Sword_Smash'"));
	if (SwordSmashSoundCueRef.Succeeded())
	{
		SwordSmashSoundCue = SwordSmashSoundCueRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> DefenseSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Character_Defense.Character_Defense'"));
	if (DefenseSoundCueRef.Succeeded())
	{
		DefenseSoundCue = DefenseSoundCueRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> DeadSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Dead.Dead'"));
	if (DeadSoundCueRef.Succeeded())
	{
		DeadSoundCue = DeadSoundCueRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> FootstepSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Footstep.Footstep'"));
	if (FootstepSoundCueRef.Succeeded())
	{
		FootstepSoundCue = FootstepSoundCueRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> LandSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Land.Land'"));
	if (LandSoundCueRef.Succeeded())
	{
		LandSoundCue = LandSoundCueRef.Object;
	}
}

void URPGAnimInstance::AnimNotify_Footstep()
{
	UGameplayStatics::PlaySound2D(this, FootstepSoundCue);
}

void URPGAnimInstance::AnimNotify_Land()
{
	Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	UGameplayStatics::PlaySound2D(this, LandSoundCue);
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
	UGameplayStatics::PlaySound2D(this, SwordSmashSoundCue);
}

void URPGAnimInstance::JumpToSectionAttackMontage(uint8 AttackCnt)
{
	FName NextAttackSection = *FString::Printf(TEXT("Attack%d"), AttackCnt);
	Montage_JumpToSection(NextAttackSection, AttackMontage);
}

void URPGAnimInstance::PlayDefenseMontage()
{
	Montage_Play(DefenseMontage);
	UGameplayStatics::PlaySound2D(this, DefenseSoundCue);
}

void URPGAnimInstance::PlayDeadMontage()
{
	StopAllMontages(0.f);
	Montage_Play(DeadMontage);
	UGameplayStatics::PlaySound2D(this, DeadSoundCue);
}

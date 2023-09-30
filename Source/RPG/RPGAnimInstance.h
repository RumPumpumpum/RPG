// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	URPGAnimInstance();

// 이동
public:
	UFUNCTION()
	void AnimNotify_Footstep();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCue)
	TObjectPtr<class USoundCue> FootstepSoundCue;

// 착지
public:
	UFUNCTION()
	void AnimNotify_Land();
	UFUNCTION()
	void AnimNotify_LandFinish();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCue)
	TObjectPtr<class USoundCue> LandSoundCue;

// 캐릭터 무브먼트 설정
protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 소유 캐릭터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// 캐릭터 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsInAir : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

// 공격
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

public:
	UFUNCTION()
	void PlayAttackMontage();
	UFUNCTION()
	void JumpToSectionAttackMontage(uint8 AttackCnt);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCue)
	TObjectPtr<class USoundCue> AttackSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCue)
	TObjectPtr<class USoundCue> SwordSmashSoundCue;

public:
	FORCEINLINE UAnimMontage* GetAttackMontage() { return AttackMontage; }

// 죽음
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

public:
	UFUNCTION()
	void PlayDeadMontage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCue)
	TObjectPtr<class USoundCue> DeadSoundCue;

// 방어
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DefenseMontage;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCue)
	TObjectPtr<class USoundCue> DefenseSoundCue;

public:
	UFUNCTION()
	void PlayDefenseMontage();
	FORCEINLINE UAnimMontage* GetDefenseMontage() { return DefenseMontage; }
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPGEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	URPGEnemyAnimInstance();

// ĳ���� �����Ʈ ����
protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

// ���� ĳ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

// ĳ���� ����
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

// ���� ����
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

public:
	UFUNCTION()
	void PlayAttackMontage();

	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void AnimNotify_AttackFinish();

private:
	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsAttacking;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCue)
	TObjectPtr<class USoundCue> AttackSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCue)
	TObjectPtr<class USoundCue> SwordSmashSoundCue;

public:
	FORCEINLINE UAnimMontage* GetAttackMontage() { return AttackMontage; }
	FORCEINLINE void SetIsAttacking(bool currentState) { bIsAttacking = currentState; }
	FORCEINLINE bool GetIsAttacking() { return bIsAttacking; }

// ����
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCue)
	TObjectPtr<class USoundCue> DeadSoundCue;

public:
	UFUNCTION()
	void PlayDeadMontage();

public:
	FORCEINLINE UAnimMontage* GetDeadMontage() { return DeadMontage; }

// �����̻�
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAcess = "ture"))
	bool Stunned;

public:
	FORCEINLINE void SetStunned(bool currentState) { Stunned = currentState; }
	FORCEINLINE bool GetStunned() { return Stunned; }
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "RPGAnimationAttackInterface.h"
#include "RPGCharacter.generated.h"

UCLASS()
class RPG_API ARPGCharacter : public ACharacter, public IRPGAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// 카메라
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// 입력
protected:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Attack();

// 애님 인스턴스
private:
	UPROPERTY()
	TObjectPtr<class URPGAnimInstance> AnimInstance;

// 공격 관련
public:
	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	// 캐릭터의 공격관련 상태
	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsAttacking;
	UPROPERTY(VisibleAnywhere, Category = State)
	bool bCanNextAttack;

	// 연속공격을 위한 카운트
	UPROPERTY(VisibleAnywhere, Category = State)
	uint8 AttackCnt;

// 공격 충돌 체크 인터페이스
protected:
	virtual void AttackHitCheck() override;
};

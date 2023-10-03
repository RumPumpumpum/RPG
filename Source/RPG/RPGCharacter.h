// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "RPGAnimationAttackInterface.h"
#include "RPGWidgetInterface.h"
#include "RPGAnimationDefenseInterface.h"
#include "RPGCharacter.generated.h"

UCLASS()
class RPG_API ARPGCharacter : public ACharacter, public IRPGAnimationAttackInterface, public IRPGWidgetInterface, public IRPGAnimationDefenseInterface
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
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCameraComp;

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

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DefenseAction;

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Attack();
	void Defense();

// 애님 인스턴스
private:
	UPROPERTY()
	TObjectPtr<class URPGAnimInstance> AnimInstance;

public:
	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);

// 공격
private:
	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsAttacking;
	UPROPERTY(VisibleAnywhere, Category = State)
	bool bCanNextAttack;

	// 연속공격을 위한 카운트
	UPROPERTY(VisibleAnywhere, Category = State)
	uint8 AttackCnt;

protected:
	// 공격 충돌 관련
	virtual void AttackHitCheck() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	TObjectPtr<class UParticleSystem> HitParticle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCue)
	TObjectPtr<class USoundCue> SwordHitSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCue)
	TObjectPtr<class USoundCue> DefenseSuccessSoundCue;

// 방어 관련
protected:
	virtual void DefenseHitCheck() override;
	virtual bool ApplyStun() override;

	UPROPERTY(VisibleAnywhere, Category = State)
	bool bIsDefensing;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	TObjectPtr<class UParticleSystem> DefenseParticle;

// 죽음
protected:
	void SetDead();

// 스탯
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class URPGCharacterStatComponent> StatComp;

// UI 
protected:
	virtual void SetupWidget(class URPGUserWidget* InUserWidget) override;

//체력 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class URPGWidgetComponent> HpBarWidgetComp;

//스탯 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class URPGWidgetComponent> StatWidgetComp;
};
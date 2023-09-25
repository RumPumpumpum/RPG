// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGWidgetInterface.h"
#include "RPGEnemyAIInterface.h"
#include "RPGAnimationAttackInterface.h"
#include "RPGEnemy.generated.h"

UCLASS()
class RPG_API ARPGEnemy : public ACharacter, public IRPGAnimationAttackInterface, public IRPGWidgetInterface, public IRPGEnemyAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// 애님 인스턴스
private:
	UPROPERTY()
	TObjectPtr<class URPGEnemyAnimInstance> AnimInstance;

// 받은 데미지 처리
protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

// 죽음
	void SetDead();

// 스탯
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class URPGEnemyStatComponent> StatComp;

// UI 
protected:
	virtual void SetupWidget(class URPGUserWidget* InUserWidget) override;

//체력 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class URPGWidgetComponent> HpBarComp;

// AI
protected:
	// 범위 관련
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRadius() override;
	virtual float GetAIAttackRange() override;

	// 공격 관련
	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual void SetAIAttackFinishedDelegate(const FAIAttackFinished& AIAttackFinished) override;
	virtual void StartAIAttack() override;

	FAIAttackFinished AttackFinished;

//공격 충돌 관련
protected:
	virtual void AttackHitCheck() override;

};

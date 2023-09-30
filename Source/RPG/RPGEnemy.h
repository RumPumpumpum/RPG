// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGWidgetInterface.h"
#include "RPGEnemyAIInterface.h"
#include "RPGAnimationAttackInterface.h"
#include "RPGAnimationDefenseInterface.h"
#include "RPGEnemy.generated.h"

UCLASS()
class RPG_API ARPGEnemy : public ACharacter, public IRPGAnimationAttackInterface, public IRPGAnimationDefenseInterface, public IRPGEnemyAIInterface, public IRPGWidgetInterface
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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	TObjectPtr<class UParticleSystem> HitParticle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCue)
	TObjectPtr<class USoundCue> SwordHitSoundCue;


// 죽음
protected:
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

// 공격 관련
protected:
	virtual void AttackHitCheck() override;

// 방어 관련
protected:
	virtual void DefenseHitCheck() override;
	virtual bool ApplyStun() override;

// 리스폰 관련
protected:
	void RespawnTimer();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	TObjectPtr<class UParticleSystem> RespawnParticle;

private:
	FVector InitialLocation;
	FRotator InitialRotation;

};

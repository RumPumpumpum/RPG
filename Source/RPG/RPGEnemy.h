// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGWidgetInterface.h"
#include "RPGEnemy.generated.h"

UCLASS()
class RPG_API ARPGEnemy : public ACharacter, public IRPGWidgetInterface
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

// �ִ� �ν��Ͻ�
private:
	UPROPERTY()
	TObjectPtr<class URPGEnemyAnimInstance> AnimInstance;

// ���� ������ ó��
protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

// ����
	void SetDead();

// ����
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class URPGEnemyStatComponent> StatComp;

// UI 
protected:
	virtual void SetupWidget(class URPGUserWidget* InUserWidget) override;

//ü�� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class URPGWidgetComponent> HpBarComp;

};

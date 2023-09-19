// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemy.h"
#include "Components/CapsuleComponent.h"
#include "RPGEnemyAnimInstance.h"
#include "Engine/DamageEvents.h"
#include "RPGEnemyStatComponent.h"
#include "RPGWidgetComponent.h"
#include "RPGHpBarWidget.h"


// Sets default values
ARPGEnemy::ARPGEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ĸ��
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("RPGCapsule"));

	// ĳ���� �޽�
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// ���̷�Ż �޽�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux'"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Enemy/ABP_RPGEnemy.ABP_RPGEnemy_C"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	// ���� ������Ʈ ���� �ʱ�ȭ
	StatComp = CreateDefaultSubobject<URPGEnemyStatComponent>(TEXT("Stat"));

	// ���� ������Ʈ ���� �ʱ�ȭ
	HpBarComp = CreateDefaultSubobject<URPGWidgetComponent>(TEXT("HpBar"));

	HpBarComp->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBarComp->SetWidgetClass(HpBarWidgetRef.Class);
		HpBarComp->SetupAttachment(GetMesh());
		HpBarComp->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarComp->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBarComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called when the game starts or when spawned
void ARPGEnemy::BeginPlay()
{
	Super::BeginPlay();

	// �ִ� �ν��Ͻ� �ʱ�ȭ
	AnimInstance = Cast<URPGEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	// ������ OnHpZero ��������Ʈ�� ����
	StatComp->OnHpZero.AddUObject(this, &ARPGEnemy::SetDead);
	
}

// Called every frame
void ARPGEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ARPGEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	StatComp->ApplyDamege(DamageAmount);

	return 0.0f;
}

void ARPGEnemy::SetDead()
{
}

void ARPGEnemy::SetupWidget(URPGUserWidget* InUserWidget)
{
	// HpBar ����
	URPGHpBarWidget* HpBarWidget = Cast<URPGHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(StatComp->GetMaxHp());
		HpBarWidget->UpdateHpBar(StatComp->GetCurrentHp());
		StatComp->OnHpChanged.AddUObject(HpBarWidget, &URPGHpBarWidget::UpdateHpBar);
	}
}


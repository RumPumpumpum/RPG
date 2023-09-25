// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "RPGEnemyAnimInstance.h"
#include "RPGEnemyStatComponent.h"
#include "RPGWidgetComponent.h"
#include "RPGHpBarWidget.h"
#include "RPGAIController.h"


// Sets default values
ARPGEnemy::ARPGEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("RPGCapsule"));
	
	// 캐릭터 메쉬
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// 스켈레탈 메쉬
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

	// 스탯 컴포넌트 관련 초기화
	StatComp = CreateDefaultSubobject<URPGEnemyStatComponent>(TEXT("Stat"));

	// 위젯 컴포넌트 관련 초기화
	HpBarComp = CreateDefaultSubobject<URPGWidgetComponent>(TEXT("HpBar"));
	
	HpBarComp->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/WBP_EnemyHpBar.WBP_EnemyHpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBarComp->SetWidgetClass(HpBarWidgetRef.Class);
		HpBarComp->SetupAttachment(GetMesh());
		HpBarComp->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarComp->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBarComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// AI
	AIControllerClass = ARPGAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ARPGEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 애님 인스턴스 초기화
	AnimInstance = Cast<URPGEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	// 스텟의 OnHpZero 델리게이트를 구독
	StatComp->OnHpZero.AddUObject(this, &ARPGEnemy::SetDead);

	// 몽타주가 끝날 때 함수를 호출하는 델리게이트	
	AnimInstance->OnMontageEnded.AddDynamic(this, &ARPGEnemy::MontageEnded);
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
	ARPGAIController* AIController = Cast<ARPGAIController>(Controller);
	// AI 중단
	if (AIController)
	{
		AIController->StopAI();
	}

	AnimInstance->PlayDeadMontage();
	SetActorEnableCollision(false);
}

void ARPGEnemy::SetupWidget(URPGUserWidget* InUserWidget)
{
	// HpBar 위젯
	URPGHpBarWidget* HpBarWidget = Cast<URPGHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(StatComp->GetMaxHp());
		HpBarWidget->UpdateHpBar(StatComp->GetCurrentHp());
		StatComp->OnHpChanged.AddUObject(HpBarWidget, &URPGHpBarWidget::UpdateHpBar);
	}
}

float ARPGEnemy::GetAIPatrolRadius()
{
	return StatComp->GetPatrolRadius();
}

float ARPGEnemy::GetAIDetectRadius()
{
	return StatComp->GetDetectRadius();
}

float ARPGEnemy::GetAIAttackRange()
{
	return StatComp->GetAttackRadius();
}

void ARPGEnemy::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 공격 몽타주 종료
	if (Montage == AnimInstance->GetAttackMontage())
	{
		// 공격 종료시 상태정보
		AnimInstance->SetIsAttacking(false);

		// 델리게이트
		AttackFinished.ExecuteIfBound();
	}
}

void ARPGEnemy::SetAIAttackFinishedDelegate(const FAIAttackFinished& AIAttackFinished)
{
	AttackFinished = AIAttackFinished;
}

void ARPGEnemy::StartAIAttack()
{
	AnimInstance->PlayAttackMontage();
}

void ARPGEnemy::AttackHitCheck()
{
	// 충돌검사 매개변수 설정
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float  AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// 충돌검사
	bool bHitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);
	if (bHitDetected)
	{
		// 감지된 모든 Pawn에 대해서 검사를 수행
		for (auto const& OutHitResult : OutHitResults)
		{
			FDamageEvent DamageEvent;
			OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		}
	}

	// !충돌판정 테스트!
#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = bHitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
#endif
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "RPGEnemyAnimInstance.h"
#include "RPGEnemyStatComponent.h"
#include "RPGWidgetComponent.h"
#include "RPGHpBarWidget.h"
#include "RPGAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"



// Sets default values
ARPGEnemy::ARPGEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ĸ��
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("RPGCapsule"));
	
	// ĳ���� �޽�
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
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

	// ���� ť ����
	static ConstructorHelpers::FObjectFinder<USoundCue> SwordHitSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Sword_Hit.Sword_Hit'"));
	if (SwordHitSoundCueRef.Succeeded())
	{
		SwordHitSoundCue = SwordHitSoundCueRef.Object;
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

	// ��Ÿ�ְ� ���� �� �Լ��� ȣ���ϴ� ��������Ʈ	
	AnimInstance->OnMontageEnded.AddDynamic(this, &ARPGEnemy::MontageEnded);

	// �ʱ� ��ġ ����
	InitialLocation = GetActorLocation();
	InitialRotation = GetActorRotation();
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

	StatComp->ApplyDamage(DamageAmount);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, this->GetActorLocation());
	
	UGameplayStatics::PlaySound2D(this, SwordHitSoundCue);

	return 0.0f;
}

void ARPGEnemy::SetDead()
{
	ARPGAIController* AIController = Cast<ARPGAIController>(Controller);
	// AI �ߴ�
	if (AIController)
	{
		AIController->StopAI();
	}

	AnimInstance->PlayDeadMontage();
	SetActorEnableCollision(false);

	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ARPGEnemy::RespawnTimer, StatComp->GetRespawnTime(), false);
}

void ARPGEnemy::SetupWidget(URPGUserWidget* InUserWidget)
{
	// HpBar ����
	URPGHpBarWidget* HpBarWidget = Cast<URPGHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(StatComp->GetMaxHp());
		HpBarWidget->UpdateHpBar(StatComp->GetCurrentHp(), StatComp->GetMaxHp());
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
	return StatComp->GetAttackRange();
}

void ARPGEnemy::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// ���� ��Ÿ�� ����
	if (Montage == AnimInstance->GetAttackMontage())
	{
		// ���� ����� ��������
		AnimInstance->SetIsAttacking(false);

		// ��������Ʈ
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
	// �浹�˻� �Ű����� ����
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = StatComp->GetAttackRange(); // ������ �����ϴ� �Ÿ�
	const float AttackRadius = AttackRange * 0.5f; // ������ ��� �Ÿ�
	const float  AttackDamage = StatComp->GetAttackDamage(); // ������

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// �浹�˻�
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
		// ������ ��� Pawn�� ���ؼ� �˻縦 ����
		for (auto const& OutHitResult : OutHitResults)
		{
			FDamageEvent DamageEvent;

			OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		}
	}
}

void ARPGEnemy::DefenseHitCheck()
{
	// �浹�˻� �Ű����� ����
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Defense), false, this);

	// ���� ����
	const float DefenseRange = StatComp->GetDefenseRange();
	const float DefenseRadius = DefenseRange * 0.5f;

	// ���� ���� ����
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * DefenseRange;

	// �浹�˻�
	bool bHitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(DefenseRadius),
		Params);
	if (bHitDetected)
	{
		for (auto const& OutHitResult : OutHitResults)
		{
			IRPGAnimationDefenseInterface* WidgetInterface;
			WidgetInterface = Cast<IRPGAnimationDefenseInterface>(OutHitResult.GetActor());
			WidgetInterface->ApplyStun();
		}
	}
}

bool ARPGEnemy::ApplyStun()
{
	if (AnimInstance->GetIsAttacking())
	{
		// �������� ��Ÿ�� ���� �� ����
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->SetStunned(true);
		AnimInstance->SetIsAttacking(false);

		//  ���� ������ ���� Ÿ�̸�
		const float StunTime = 5.0f;
		FTimerHandle StunTimerHandle;
		GetWorldTimerManager().SetTimer(
			StunTimerHandle, 
			[&]() {
				AnimInstance->StopAllMontages(0.0f);
				AnimInstance->SetStunned(false);

				ARPGAIController* AIController = Cast<ARPGAIController>(Controller);
				UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
				BlackboardComp->SetValueAsBool(FName(TEXT("Stunned")), false);
			}, 
			StunTime, 
			false);

		// ���� ó���� ���� ������ ������Ʈ ����
		ARPGAIController* AIController = Cast<ARPGAIController>(Controller);
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		BlackboardComp->SetValueAsBool(FName(TEXT("Stunned")), true);

		return true;
	}

	return false;
}

void ARPGEnemy::RespawnTimer()
{
	Destroy();
	GetWorld()->SpawnActor(this->GetClass(), &InitialLocation, &InitialRotation);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RespawnParticle, InitialLocation);
}

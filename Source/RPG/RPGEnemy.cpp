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
#include "RPGBattleRewardInterface.h"

// Sets default values
ARPGEnemy::ARPGEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("RPGCapsule"));
	
	// 캐릭터 메쉬
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
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

	// 사운드 큐 설정
	static ConstructorHelpers::FObjectFinder<USoundCue> SwordHitSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Sword_Hit.Sword_Hit'"));
	if (SwordHitSoundCueRef.Succeeded())
	{
		SwordHitSoundCue = SwordHitSoundCueRef.Object;
	}

	// 공격 몽타주 로드
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Enemy/Animations/AM_EnemyAttack.AM_EnemyAttack'"));
	if (AttackMontageRef.Succeeded())
	{
		AttackMontage = AttackMontageRef.Object;
	}
}

// Called when the game starts or when spawned
void ARPGEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 애님 인스턴스 초기화
	AnimInstance = Cast<URPGEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	// 게임 인스턴스 초기화
	RPGGameInstance = Cast<URPGGameInstance>(GetGameInstance());

	// 스텟의 OnHpZero 델리게이트를 구독
	StatComp->OnHpZero.AddUObject(this, &ARPGEnemy::SetDead);

	// 몽타주가 끝날 때 함수를 호출하는 델리게이트	
	AnimInstance->OnMontageEnded.AddDynamic(this, &ARPGEnemy::MontageEnded);

	// 초기 위치 저장
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
	// AI 중단
	if (AIController)
	{
		AIController->StopAI();
	}
	
	// 스턴 타이머가 걸려있다면 제거
	if (StunTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(StunTimerHandle);
		StunTimerHandle.Invalidate();
	}

	AnimInstance->PlayDeadMontage();
	SetActorEnableCollision(false);

	// 리스폰
	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ARPGEnemy::RespawnTimer, StatComp->GetRespawnTime(), false);
	
	// 퀘스트 카운트 증가
	TArray<FRPGQuestData>& QuestTable = RPGGameInstance->QuestTable;
	for (int32 i = 0; i < QuestTable.Num(); ++i)
	{
		// 수락한 퀘스트를 확인
		if (QuestTable[i].Accepted)
		{
			QuestTable[i].KilledCnt++;
		}
	}

	// 사망시 스탯 포인트 리워드 지급, 체력회복
	AController* CharacterController = GetWorld()->GetFirstPlayerController();
	IRPGBattleRewardInterface* BattleRewardInterface = Cast<IRPGBattleRewardInterface>(CharacterController->GetPawn());
	BattleRewardInterface->StatPointReward(StatComp->GetRewardStatPoint());
	BattleRewardInterface->HPRegen(StatComp->GetRewardHP());
}

void ARPGEnemy::SetupWidget(URPGUserWidget* InUserWidget)
{
	// HpBar 위젯
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
	// 공격 몽타주 종료
	if (Montage == AttackMontage)
	{
		// 공격 종료시 상태정보
		AnimInstance->SetIsAttacking(false);

		// 공격 종료 델리게이트
		AttackFinished.ExecuteIfBound();
	}
}

void ARPGEnemy::SetAIAttackFinishedDelegate(const FAIAttackFinished& AIAttackFinished)
{
	AttackFinished = AIAttackFinished;
}

void ARPGEnemy::StartAIAttack()
{
	AnimInstance->Montage_Play(AttackMontage, 1.0f);
}

void ARPGEnemy::AttackHitCheck()
{
	// 충돌검사 매개변수 설정
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = StatComp->GetAttackRange(); // 공격을 시작하는 거리
	const float AttackRadius = AttackRange * 0.5f; // 공격이 닿는 거리
	const float  AttackDamage = StatComp->GetAttackDamage(); // 데미지

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
			// 몬스터를 공격했다면 무시
			if (OutHitResult.GetActor()->IsA(ARPGEnemy::StaticClass()))
			{
				continue;
			}

			FDamageEvent DamageEvent;
			OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);

		}
	}
}

void ARPGEnemy::DefenseHitCheck()
{
	// 충돌검사 매개변수 설정
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Defense), false, this);

	// 스텟 설정
	const float DefenseRange = StatComp->GetDefenseRange();
	const float DefenseRadius = DefenseRange * 0.5f;

	// 공격 범위 설정
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * DefenseRange;

	// 충돌검사
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
		// 진행중인 몽타주 종료 후 스턴
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->SetStunned(true);
		AnimInstance->SetIsAttacking(false);

		//  스턴 해제를 위한 타이머
		GetWorldTimerManager().SetTimer(
			StunTimerHandle, 
			[&]() {
				AnimInstance->StopAllMontages(0.0f);
				AnimInstance->SetStunned(false);

				ARPGAIController* AIController = Cast<ARPGAIController>(Controller);
				UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
				BlackboardComp->SetValueAsBool(FName(TEXT("Stunned")), false);
			}, 
			StatComp->GetStunTime(),
			false);

		// 스턴 처리를 위해 블랙보드 컴포넌트 얻어옴
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

#include "RPGCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "RPGAnimInstance.h"
#include "Engine/DamageEvents.h"
#include "RPGCharacterStatComponent.h"
#include "RPGWidgetComponent.h"
#include "RPGHpBarWidget.h"
#include "RPGStatWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/DataTable.h"
#include "RPGQuestMainWidget.h"
#include "RPGHUDWidget.h"


// Sets default values
ARPGCharacter::ARPGCharacter()
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
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	// 애님 인스턴스
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Character/ABP_RPGCharacter.ABP_RPGCharacter_C"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	// 캐릭터 무브먼트
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MaxAcceleration = 1500.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	// 카메라
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 1000.f;
	SpringArmComp->bUsePawnControlRotation = true;

	FollowCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	FollowCameraComp->bUsePawnControlRotation = false;

	// 입력 액션
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>DEFAULT_CONTEXT
	(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (DEFAULT_CONTEXT.Succeeded())
		DefaultMappingContext = DEFAULT_CONTEXT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_JUMP
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	if (IA_JUMP.Succeeded())	
		JumpAction = IA_JUMP.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_LOOK
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Look.IA_Look'"));
	if (IA_LOOK.Succeeded())
		LookAction = IA_LOOK.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOVE
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
	if (IA_MOVE.Succeeded())
		MoveAction = IA_MOVE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ATTACK
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Attack.IA_Attack'"));
	if (IA_ATTACK.Succeeded())
		AttackAction = IA_ATTACK.Object;	

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_DEFENSE
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Defense.IA_Defense'"));
	if (IA_DEFENSE.Succeeded())
		DefenseAction = IA_DEFENSE.Object;

	// 공격 관련 캐릭터 상태 초기화
	bIsAttacking = false;
	bCanNextAttack = false;
	AttackCnt = 0;

	// 스탯 컴포넌트 초기화
	StatComp = CreateDefaultSubobject<URPGCharacterStatComponent>(TEXT("Stat"));

	// 위젯 컴포넌트 초기화
	HpBarWidgetComp = CreateDefaultSubobject<URPGWidgetComponent>(TEXT("HpBar"));
	StatWidgetComp = CreateDefaultSubobject<URPGWidgetComponent>(TEXT("StatWidget"));

	// 위젯 호출 및 초기화
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBarWidgetComp->SetWidgetClass(HpBarWidgetRef.Class);
		HpBarWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
		HpBarWidgetComp->SetupAttachment(GetMesh());
		HpBarWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarWidgetComp->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBarWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> StatWidgetRef(TEXT("/Game/UI/WBP_CharacterStat.WBP_CharacterStat_C"));
	if (StatWidgetRef.Class)
	{
		StatWidgetComp->SetWidgetClass(StatWidgetRef.Class);
		StatWidgetComp->SetVisibility(false);
	}

	// 퀘스트 위젯
	static ConstructorHelpers::FClassFinder<UUserWidget> QuestMainWidgetRef(TEXT("/Game/UI/WBP_QuestMain.WBP_QuestMain_C"));
	if (QuestMainWidgetRef.Succeeded())
	{
		QuestMainWidgetClass = QuestMainWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> QuestSlotWidgetRef(TEXT("/Game/UI/WBP_QuestSlot.WBP_QuestSlot_C"));
	if (QuestSlotWidgetRef.Succeeded())
	{
		QuestSlotWidgetClass = QuestSlotWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> QuestCountingWidgetRef(TEXT("/Game/UI/WBP_QuestCounting.WBP_QuestCounting_C"));
	if (QuestCountingWidgetRef.Succeeded())
	{
		QuestCountingWidgetClass = QuestCountingWidgetRef.Class;
	}

	// HUD 위젯
	static ConstructorHelpers::FClassFinder<URPGHUDWidget> RPGHUDWidgetRef(TEXT("/Game/UI/WBP_HUD.WBP_HUD_C"));
	if (RPGHUDWidgetRef.Class)
	{
		RPGHUDWidgetClass = RPGHUDWidgetRef.Class;
	}

	// 사운드 큐 설정
	static ConstructorHelpers::FObjectFinder<USoundCue> DefenseSuccessSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Defense_Success.Defense_Success'"));
	if (DefenseSuccessSoundCueRef.Succeeded())
	{
		DefenseSuccessSoundCue = DefenseSuccessSoundCueRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SwordHitSoundCueRef(TEXT("/Script/Engine.SoundCue'/Game/Audio/Cues/Sword_Hit.Sword_Hit'"));
	if (SwordHitSoundCueRef.Succeeded())
	{
		SwordHitSoundCue = SwordHitSoundCueRef.Object;
	}
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 애님 인스턴스 초기화
	AnimInstance = Cast<URPGAnimInstance>(GetMesh()->GetAnimInstance());

	// Input Mapping Context 추가
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
			(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// 몽타주가 끝날 때 함수를 호출하는 델리게이트	
	AnimInstance->OnMontageEnded.AddDynamic(this, &ARPGCharacter::MontageEnded);

	// 스텟 컴포넌트의 OnHpZero 델리게이트를 구독
	StatComp->OnHpZero.AddUObject(this, &ARPGCharacter::SetDead);

	// 초기 위치 저장
	InitialLocation = GetActorLocation();
	InitialRotation = GetActorRotation();

	// HUD 위젯 생성
	RPGHUDWidget = CreateWidget<URPGHUDWidget>(GetWorld(), RPGHUDWidgetClass);
	if (RPGHUDWidget)
	{
		RPGHUDWidget->AddToViewport();
	}
}

// Called every frame
void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jump
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Look
		EnhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Look);
		
		// Move
		EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Move);

		// Attack
		EnhancedInputComp->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Attack);
	
		// Defense
		EnhancedInputComp->BindAction(DefenseAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Defense);
	}
}

void ARPGCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// 마우스의 좌우,상하 입력을 추가
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ARPGCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 캐릭터가 좌우로만 회전
	const FRotator Rotation = Controller->GetControlRotation();	
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// 벡터 계산
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// MovementVector 값에 따라 이동
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ARPGCharacter::Attack()
{
	// 공격 예외 처리
	// 점프 중일때
	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		return;
	}

	// 방어 중일때
	if (bIsDefensing)
	{
		return;
	}

	// 이미 공격 중일때
	if (bIsAttacking)
	{
		bCanNextAttack = true;
		return;
	}


	// 몽타주 재생
	if (AnimInstance)
	{
		AnimInstance->PlayAttackMontage();
		AnimInstance->JumpToSectionAttackMontage(AttackCnt);
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AttackCnt++;
	bIsAttacking = true;

}

void ARPGCharacter::Defense()
{
	// 방어 예외 처리
	// 공격 중일때
	if (bIsAttacking)
	{
		return;
	}

	// 점프 중일 때
	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		return;
	}

	// 이미 방어 중일 때
	if (bIsDefensing)
	{
		return;
	}

	// 몽타주 재생
	if (AnimInstance)
	{
		AnimInstance->PlayDefenseMontage();
	}

	// 방어시 캐릭터의 상태 정보
	bIsDefensing = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void ARPGCharacter::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 어택 몽타주 종료
	if (Montage == AnimInstance->GetAttackMontage())
	{
		// 공격 종료시 캐릭터의 상태정보
		bIsAttacking = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		// 다음 단계 공격여부 판별을 위한 조건문
		// 공격 도중에 공격을 다시 시도하였고, 지금까지 3번 이하의 공격을 했다면 연속공격
		if (bCanNextAttack && AttackCnt <= 2)
		{
			Attack();
		}
		// 연속 공격을 시도하지 않았거나 공격횟수가 3회를 초과하였을 경우, 공격 카운트를 초기화하고 몽타주 종료
		else if (!bCanNextAttack || AttackCnt >= 3)
		{
			AttackCnt = 0;
		}

		bCanNextAttack = false;
	}

	// 방어 몽타주 종료
	if (Montage == AnimInstance->GetDefenseMontage())
	{
		// 방어 종료시 캐릭터의 상태정보
		bIsDefensing = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

float ARPGCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent, 
	AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	StatComp->ApplyDamage(DamageAmount);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, this->GetActorLocation());

	UGameplayStatics::PlaySound2D(this, SwordHitSoundCue);

	return 0.0f;
}

void ARPGCharacter::DefenseHitCheck()
{
	// 충돌검사 매개변수 설정
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Defense), false, this);

	// 스텟 설정
	const float DefenseRange = StatComp->GetDefenseRange();
	const float DefenseRadius = DefenseRange * 0.5f;
	
	// 방어 범위 설정
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * DefenseRange;

	// 방어 결과
	bool DefenseResult = false;

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
			WidgetInterface = Cast<IRPGAnimationDefenseInterface>(OutHitResult.GetActor());\
			if(WidgetInterface->ApplyStun())
			{
				DefenseResult = true;
			}
		}

		// 방어가 성공적으로 진행 되었을 경우
		if (DefenseResult)
		{
			// 파티클 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefenseParticle, Start);

			// 방어 성공 시 연출 효과
			float TimeSlowDilation = 0.3f; // 얼마만큼 슬로우
			float TimeSlowDelay = 1.0f; // 몇초동안 슬로우

			GetWorldSettings()->SetTimeDilation(TimeSlowDilation);
			SpringArmComp->TargetArmLength = 500.f;
			UGameplayStatics::PlaySound2D(this, DefenseSuccessSoundCue);
			
			FTimerHandle TimeSlowTimerHandle;
			GetWorldTimerManager().SetTimer(
				TimeSlowTimerHandle,
				[&]() {
					GetWorldSettings()->SetTimeDilation(1.0f);
					SpringArmComp->TargetArmLength = 1000.f;
				},
				TimeSlowDilation / TimeSlowDelay,
				false);
		}
	}
}

bool ARPGCharacter::ApplyStun()
{
	// TODO
	return false;
}

void ARPGCharacter::SetDead()
{
	// 이전 부활 타이머가 있다면 제거
	if (GetWorldTimerManager().IsTimerActive(RespawnTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}

	AnimInstance->PlayDeadMontage();

	// 일정 시간 후에 부활하기 위한 타이머 설정
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ARPGCharacter::Respawn, StatComp->GetRespawnTime(), false);
}

void ARPGCharacter::Respawn()
{
	// 부활 위치로 플레이어를 이동
	SetActorLocation(InitialLocation);

	// 충돌 및 이동모드 초기화
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	SetActorEnableCollision(true);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RespawnParticle, InitialLocation);
	AnimInstance->StopAllMontages(0.f);

	// 최대 체력만큼 회복
	StatComp->SetHp(StatComp->GetMaxHp());
}

void ARPGCharacter::SetupWidget(URPGUserWidget* InUserWidget)
{
	// HpBar 위젯
	URPGHpBarWidget* HpBarWidget = Cast<URPGHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{	
		HpBarWidget->SetMaxHp(StatComp->GetMaxHp());
		HpBarWidget->UpdateHpBar(StatComp->GetCurrentHp(), StatComp->GetMaxHp());
		StatComp->OnHpChanged.AddUObject(HpBarWidget, &URPGHpBarWidget::UpdateHpBar);
	}	

	// Stat 위젯
	URPGStatWidget* StatWidget = Cast<URPGStatWidget>(InUserWidget);
	if (StatWidget)
	{
		StatWidget->UpdateMaxHp(StatComp->GetMaxHp());
		StatWidget->UpdateDamage(StatComp->GetAttackDamage());
		StatWidget->UpdateStatPoint(StatComp->GetStatPoint());

		StatWidget->OnMaxHpChanged.AddUObject(StatComp, &URPGCharacterStatComponent::SetMaxHp);
		StatWidget->OnDamageChanged.AddUObject(StatComp, &URPGCharacterStatComponent::SetDamage);
	}

	// HUD 위젯
	URPGHUDWidget* HUDWidget = Cast<URPGHUDWidget>(InUserWidget);
	if (HUDWidget)
	{
		HUDWidget->UpdateHpBar(StatComp->GetCurrentHp(), StatComp->GetMaxHp());
		StatComp->OnHpChanged.AddUObject(HUDWidget, &URPGHUDWidget::UpdateHpBar);

		HUDWidget->SetQuestCountingWidgetClass(QuestCountingWidgetClass);
	}
}

void ARPGCharacter::StatPointReward(int RewardPoint)
{
	URPGStatWidget* StatWidget = Cast<URPGStatWidget>(StatWidgetComp->GetWidget());
	StatWidget->IncreaseStatPoint(RewardPoint);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RewardParticle, GetActorLocation());

	RPGHUDWidget->UpdateQuestCount();
}

void ARPGCharacter::HPRegen(float RewardHP)
{
	StatComp->SetHp(StatComp->GetCurrentHp() + RewardHP);
}

void ARPGCharacter::CreateQuestWidget()
{
	QuestMainWidget = CreateWidget<URPGQuestMainWidget>(GetWorld(), QuestMainWidgetClass);
	if (QuestMainWidget)
	{
		QuestMainWidget->AddToViewport();
	}

	QuestMainWidget->AddQuest(QuestSlotWidgetClass);
}

void ARPGCharacter::DestroyQuestWidget()
{
	QuestMainWidget->RemoveFromParent();
}

void ARPGCharacter::UpdateQuestList()
{
	RPGHUDWidget->UpdateQuestList();
}

void ARPGCharacter::AttackHitCheck()
{
	// 충돌검사 매개변수 설정
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	// 스텟 설정
	const float AttackRange = StatComp->GetAttackRange();
	const float AttackRadius = AttackRange * 0.5f;

	// 연속공격시 데미지 상승 (첫공격시 AttackCnt는 1)
	const float DamageIncrease = 15.0f;
	const float AttackDamage = StatComp->GetAttackDamage() + ((AttackCnt - 1) * DamageIncrease);

	// 공격 범위 설정
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
}




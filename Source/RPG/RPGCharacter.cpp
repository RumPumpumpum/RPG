// Fill out your copyright notice in the Description page of Project Settings.


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


// Sets default values
ARPGCharacter::ARPGCharacter()
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
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (CharacterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	// �ִ� �ν��Ͻ�
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Character/ABP_RPGCharacter.ABP_RPGCharacter_C"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	// ĳ���� �����Ʈ
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MaxAcceleration = 1500.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	// ī�޶�
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1000.f;
	SpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// �Է� �׼�
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

	// ���� ���� ĳ���� ���� �ʱ�ȭ
	bIsAttacking = false;
	bCanNextAttack = false;
	AttackCnt = 0;

	// ���� ������Ʈ ���� �ʱ�ȭ
	StatComp = CreateDefaultSubobject<URPGCharacterStatComponent>(TEXT("Stat"));

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
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	// �ִ� �ν��Ͻ� �ʱ�ȭ
	AnimInstance = Cast<URPGAnimInstance>(GetMesh()->GetAnimInstance());

	// Input Mapping Context �߰�
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
			(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// ��Ÿ�ְ� ���� �� �Լ��� ȣ���ϴ� ��������Ʈ	
	AnimInstance->OnMontageEnded.AddDynamic(this, &ARPGCharacter::MontageEnded);

	// ������ OnHpZero ��������Ʈ�� ����
	StatComp->OnHpZero.AddUObject(this, &ARPGCharacter::SetDead);
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

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Look);
		
		// Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Move);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Attack);
	}
}

void ARPGCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// ���콺�� �¿�,���� �Է��� �߰�
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ARPGCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// ĳ���Ͱ� �¿�θ� ȸ��
	const FRotator Rotation = Controller->GetControlRotation();	
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// ���� ���
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// MovementVector ���� ���� �̵�
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ARPGCharacter::Attack()
{
	// ���� ���� ó��
	// ���� ���϶�
	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		return;
	}

	// �̹� ���� ���϶�
	if (bIsAttacking)
	{
		bCanNextAttack = true;
		return;
	}

	// ��Ÿ�� ���
	if (AnimInstance)
	{
		AnimInstance->PlayAttackMontage();
		AnimInstance->JumpToSectionAttackMontage(AttackCnt);
	}

	// ���ݽ� ĳ������ ���� ����
	bIsAttacking = true;
	AttackCnt++;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void ARPGCharacter::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// ���� ��Ÿ�� ����
	if (Montage == AnimInstance->GetAttackMontage())
	{
		// ���� ����� ĳ������ ��������
		bIsAttacking = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		// ���� �ܰ� ���ݿ��� �Ǻ��� ���� ���ǹ�
		// ���� ���߿� ������ �ٽ� �õ��Ͽ���, ���ݱ��� 3�� ������ ������ �ߴٸ� ���Ӱ���
		if (bCanNextAttack && AttackCnt <= 2)
		{
			Attack();
		}
		// ���� ������ �õ����� �ʾҰų� ����Ƚ���� 3ȸ�� �ʰ��Ͽ��� ���, ���� ī��Ʈ�� �ʱ�ȭ�ϰ� ��Ÿ�� ����
		else if (!bCanNextAttack || AttackCnt >= 3)
		{
			AttackCnt = 0;
		}

		bCanNextAttack = false;
	}
}

float ARPGCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent, 
	AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	StatComp->ApplyDamege(DamageAmount);

	return 0.0f;
}

void ARPGCharacter::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);
	AnimInstance->PlayDeadMontage();
}

void ARPGCharacter::SetupWidget(URPGUserWidget* InUserWidget)
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

void ARPGCharacter::AttackHitCheck()
{
	// �浹�˻� �Ű����� ����
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	FHitResult OutHitResult;

	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float  AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// �浹�˻�
	bool HitDetected = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

	// �浹���� �׸���
#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
#endif
}




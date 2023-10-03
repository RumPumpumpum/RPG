// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGStatWidget.h"
#include "Components/TextBlock.h"
#include "RPGWidgetInterface.h"


URPGStatWidget::URPGStatWidget()
{
	MaxHp = 0.0f;
	Damage = 0.0f;
	StatPoint = 0.0f;
}

void URPGStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MaxHpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxHpText")));
	ensure(MaxHpText);

	DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DamageText")));
	ensure(DamageText);

	StatPointText = Cast<UTextBlock>(GetWidgetFromName(TEXT("StatPointText")));
	ensure(StatPointText);

	// 액터가 위젯 인터페이스를 구현하고 있는지 확인하고 구현하고 있다면
	// 해당 인터페이스를 사용할 수 있도록 인터페이스 포인터로 반환 
	IRPGWidgetInterface* WidgetInterface = Cast<IRPGWidgetInterface>(OwningActor);
	if (WidgetInterface)
	{
		WidgetInterface->SetupWidget(this);
	}
}

void URPGStatWidget::UpdateMaxHp(float NewMaxHp)
{
	MaxHp = NewMaxHp;

	FString MaxHpString = FString::Printf(TEXT("%.1f"), MaxHp);
	MaxHpText->SetText(FText::FromString(MaxHpString));
}

void URPGStatWidget::UpdateDamage(float NewDamage)
{
	Damage = NewDamage;

	FString DamageString = FString::Printf(TEXT("%.1f"), Damage);
	DamageText->SetText(FText::FromString(DamageString));
}

void URPGStatWidget::IncreaseMaxHp(float Value)
{
	MaxHp += Value;
	UpdateMaxHp(MaxHp);

	OnMaxHpChanged.Broadcast(MaxHp);
}

void URPGStatWidget::IncreaseDamage(float Value)
{
	Damage += Value;
	UpdateDamage(Damage);

	OnDamageChanged.Broadcast(Damage);
}

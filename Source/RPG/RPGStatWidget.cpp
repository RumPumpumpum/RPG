// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGStatWidget.h"
#include "Components/TextBlock.h"
#include "RPGWidgetInterface.h"


URPGStatWidget::URPGStatWidget()
{
	MaxHp = -1.0f;
	Damage = -1.0f;
}

void URPGStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MaxHpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxHpText")));
	ensure(MaxHpText);

	DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxHpText")));
	ensure(DamageText);

	// ���Ͱ� ���� �������̽��� �����ϰ� �ִ��� Ȯ���ϰ� �����ϰ� �ִٸ�
	// �ش� �������̽��� ����� �� �ֵ��� �������̽� �����ͷ� ��ȯ 
	IRPGWidgetInterface* WidgetInterface = Cast<IRPGWidgetInterface>(OwningActor);
	if (WidgetInterface)
	{
		WidgetInterface->SetupWidget(this);
	}
}

void URPGStatWidget::UpdateMaxHp(float NewMaxHp)
{
	MaxHp = NewMaxHp;

	// MaxHp ���� ���ڿ��� ��ȯ�Ͽ� MaxHpText�� ����
	FString MaxHpString = FString(TEXT("%f"), MaxHp);
	MaxHpText->SetText(FText::FromString(MaxHpString));
}

void URPGStatWidget::UpdateDamage(float NewDamage)
{
	Damage = NewDamage;

	// MaxHp ���� ���ڿ��� ��ȯ�Ͽ� MaxHpText�� ����
	FString DamageString = FString(TEXT("%f"), Damage);
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

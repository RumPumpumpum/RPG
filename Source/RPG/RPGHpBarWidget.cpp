// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "RPGWidgetInterface.h"

URPGHpBarWidget::URPGHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void URPGHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� ������Ʈ ������
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpBar);

	// ���Ͱ� ���� �������̽��� �����ϰ� �ִ��� Ȯ���ϰ� �����ϰ� �ִٸ�
	// �ش� �������̽��� ����� �� �ֵ��� �������̽� �����ͷ� ��ȯ 
	IRPGWidgetInterface* WidgetInterface = Cast<IRPGWidgetInterface>(OwningActor);
	if (WidgetInterface)
	{
		WidgetInterface->SetupWidget(this);
	}
}

void URPGHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpBar)
	{
		HpBar->SetPercent(NewCurrentHp / MaxHp);
	}
}

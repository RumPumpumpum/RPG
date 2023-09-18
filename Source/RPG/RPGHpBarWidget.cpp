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

	// 위젯 컴포넌트 가져옴
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpBar);

	// 액터가 위젯 인터페이스를 구현하고 있는지 확인하고 구현하고 있다면
	// 해당 인터페이스를 사용할 수 있도록 인터페이스 포인터로 반환 
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

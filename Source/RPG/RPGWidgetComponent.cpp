// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGWidgetComponent.h"
#include "RPGUserWidget.h"

void URPGWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// 위젯 컴포넌트를 소유중인 액터 정보를 위젯 컴포넌트가 소유중인 위젯으로 보냄
	URPGUserWidget* RPGUserWidget = Cast<URPGUserWidget>(GetWidget());
	if (RPGUserWidget)
	{
		RPGUserWidget->SetOwningActor(GetOwner());
	}
}

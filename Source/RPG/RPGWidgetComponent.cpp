// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGWidgetComponent.h"
#include "RPGUserWidget.h"

void URPGWidgetComponent::InitWidget()
{
	Super::InitWidget();

	URPGUserWidget* RPGUserWidget = Cast<URPGUserWidget>(GetWidget());
	if (RPGUserWidget)
	{
		RPGUserWidget->SetOwningActor(GetOwner());
	}
}

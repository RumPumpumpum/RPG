// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGGameMode.h"
#include "RPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARPGGameMode::ARPGGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("/Game/Character/BP_RPGCharacter.BP_RPGCharacter_C"));
	if (PlayerPawnClass.Class)
	{
		DefaultPawnClass = PlayerPawnClass.Class;
	}
}

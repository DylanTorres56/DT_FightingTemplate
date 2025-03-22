// Copyright Epic Games, Inc. All Rights Reserved.

#include "DT_FightingTemplateGameMode.h"
#include "DT_FightingTemplateCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADT_FightingTemplateGameMode::ADT_FightingTemplateGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

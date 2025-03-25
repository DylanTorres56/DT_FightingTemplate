// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DT_FightingTemplateCharacter.h"
#include "DT_FightingTemplateGameMode.generated.h"

UCLASS(minimalapi)
class ADT_FightingTemplateGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADT_FightingTemplateGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ADT_FightingTemplateCharacter* leftSidePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ADT_FightingTemplateCharacter* rightSidePlayer;
};




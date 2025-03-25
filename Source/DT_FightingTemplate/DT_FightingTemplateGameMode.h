// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DT_FightingTemplateCharacter.h"
#include "DT_FightingTemplateGameMode.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	VE_Manny			UMETA(DisplayName = "Manny"),
	VE_ShingoNGPC3D		UMETA(DisplayName = "Shingo (NGPC 3D)")
};

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




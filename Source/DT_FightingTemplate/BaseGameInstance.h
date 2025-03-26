// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	VE_Manny			UMETA(DisplayName = "Manny"),
	VE_ShingoNGPC3D		UMETA(DisplayName = "Shingo (NGPC 3D)")
};

UCLASS()
class DT_FIGHTINGTEMPLATE_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ECharacterClass characterClass;
};
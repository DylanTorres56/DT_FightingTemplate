// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DT_FightingTemplateCharacter.h"
#include "ShingoNGPC3D.generated.h"

UCLASS()
class DT_FIGHTINGTEMPLATE_API AShingoNGPC3D : public ADT_FightingTemplateCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties.
	AShingoNGPC3D();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float deltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};


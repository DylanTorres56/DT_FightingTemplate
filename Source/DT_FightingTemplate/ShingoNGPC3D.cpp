// Fill out your copyright notice in the Description page of Project Settings.
#include "ShingoNGPC3D.h"

// Sets default values for this character's properties.
AShingoNGPC3D::AShingoNGPC3D() 
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned.
void AShingoNGPC3D::BeginPlay() 
{
	Super::BeginPlay();
}

// Called every frame
void AShingoNGPC3D::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

// Called to bind functionality to input
void AShingoNGPC3D::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
// Copyright Epic Games, Inc. All Rights Reserved.

#include "DT_FightingTemplateCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ADT_FightingTemplateCharacter::ADT_FightingTemplateCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	playerHealth = 1.00f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADT_FightingTemplateCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADT_FightingTemplateCharacter::MoveRight);

	PlayerInputComponent->BindAction("AttackA", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackA);
	// PlayerInputComponent->BindAction("AttackA", IE_Released, this, &ADT_FightingTemplateCharacter::StopAttackA);
	PlayerInputComponent->BindAction("AttackB", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackB);
	// PlayerInputComponent->BindAction("AttackB", IE_Released, this, &ADT_FightingTemplateCharacter::StopAttackB);
	PlayerInputComponent->BindAction("AttackC", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackC);
	// PlayerInputComponent->BindAction("AttackC", IE_Released, this, &ADT_FightingTemplateCharacter::StopAttackC);
	PlayerInputComponent->BindAction("AttackD", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackD);
	// PlayerInputComponent->BindAction("AttackD", IE_Released, this, &ADT_FightingTemplateCharacter::StopAttackD);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADT_FightingTemplateCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADT_FightingTemplateCharacter::TouchStopped);
}

void ADT_FightingTemplateCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ADT_FightingTemplateCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ADT_FightingTemplateCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ADT_FightingTemplateCharacter::StartAttackA()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack A called!"));
}

void ADT_FightingTemplateCharacter::StartAttackB()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack B called!"));
}

void ADT_FightingTemplateCharacter::StartAttackC()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack C called!"));
}

void ADT_FightingTemplateCharacter::StartAttackD()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack D called!"));
	TakeDamage(.05f);
}

void ADT_FightingTemplateCharacter::TakeDamage(float _damageAmount) 
{
	UE_LOG(LogTemp, Warning, TEXT("We are taking damage for %f points."), _damageAmount);
	playerHealth -= _damageAmount;

	if (playerHealth <= 0.0f) 
	{
		playerHealth = 0.0f;
	}
}
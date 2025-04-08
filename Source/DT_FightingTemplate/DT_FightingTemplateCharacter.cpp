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

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	otherPlayer = nullptr;
	hurtbox = nullptr;
	directionalInput = EDirectionalInput::VE_Default;
	transform = FTransform();
	scale = FVector(0.0f, 0.0f, 0.0f);
	attackA_Used = false;
	attackB_Used = false;
	attackC_Used = false;
	attackD_Used = false;
	playerHealth = 1.00f;
	isFlipped = false;
	hasLandedAttack = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADT_FightingTemplateCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	/*if (auto gameMode = Cast<ADT_FightingTemplateGameMode>(GetWorld()->GetAuthGameMode()))
	{

	}*/


	// set up gameplay key bindings
	PlayerInputComponent->BindAction("P1_Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("P1_Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("P1_MoveRight", this, &ADT_FightingTemplateCharacter::MoveRight);

	PlayerInputComponent->BindAction("P1_AttackA", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackA);
	// PlayerInputComponent->BindAction("P1_AttackA", IE_Released, this, &ADT_FightingTemplateCharacter::StopAttackA);
	PlayerInputComponent->BindAction("P1_AttackB", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackB);
	// PlayerInputComponent->BindAction("P1_AttackB", IE_Released, this, &ADT_FightingTemplateCharacter::StopAttackB);
	PlayerInputComponent->BindAction("P1_AttackC", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackC);
	// PlayerInputComponent->BindAction("P1_AttackC", IE_Released, this, &ADT_FightingTemplateCharacter::StopAttackC);
	PlayerInputComponent->BindAction("P1_AttackD", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackD);
	// PlayerInputComponent->BindAction("P1_AttackD", IE_Released, this, &ADT_FightingTemplateCharacter::StopAttackD);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADT_FightingTemplateCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADT_FightingTemplateCharacter::TouchStopped);
}

void ADT_FightingTemplateCharacter::MoveRight(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("The directional value is: %f"), Value);
	if (Value > 0.20f) 
	{
		directionalInput = EDirectionalInput::VE_MovingRight;
	}
	else if (Value < -0.20f)
	{
		directionalInput = EDirectionalInput::VE_MovingLeft;
	}
	else 
	{
		directionalInput = EDirectionalInput::VE_Default;
	}

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
	// UE_LOG(LogTemp, Warning, TEXT("Attack A called!"));
	attackA_Used = true;
}

void ADT_FightingTemplateCharacter::StartAttackB()
{
	// UE_LOG(LogTemp, Warning, TEXT("Attack B called!"));
	attackB_Used = true;
}

void ADT_FightingTemplateCharacter::StartAttackC()
{
	// UE_LOG(LogTemp, Warning, TEXT("Attack C called!"));
	attackC_Used = true;
}

void ADT_FightingTemplateCharacter::StartAttackD()
{
	// UE_LOG(LogTemp, Warning, TEXT("Attack D called!"));	
	attackD_Used = true;
}

// P2 Functions (On Keyboard!)

void ADT_FightingTemplateCharacter::P2_StartAttackA()
{
	// UE_LOG(LogTemp, Warning, TEXT("Attack A called!"));	
	StartAttackA();
}

void ADT_FightingTemplateCharacter::P2_StartAttackB()
{
	// UE_LOG(LogTemp, Warning, TEXT("Attack B called!"));	
	StartAttackB();
}

void ADT_FightingTemplateCharacter::P2_StartAttackC()
{
	// UE_LOG(LogTemp, Warning, TEXT("Attack C called!"));	
	StartAttackC();
}

void ADT_FightingTemplateCharacter::P2_StartAttackD()
{
	// UE_LOG(LogTemp, Warning, TEXT("Attack D called!"));	
	StartAttackD();
}

void ADT_FightingTemplateCharacter::P2_Jump()
{	
	Jump();
}

void ADT_FightingTemplateCharacter::P2_StopJumping()
{
	StopJumping();
}

void ADT_FightingTemplateCharacter::P2_MoveRight(float _value)
{
	// UE_LOG(LogTemp, Warning, TEXT("Attack D called!"));	
	MoveRight(_value);
}

void ADT_FightingTemplateCharacter::TakeDamage(float _damageAmount) 
{
	UE_LOG(LogTemp, Warning, TEXT("We are taking damage for %f points."), _damageAmount);
	playerHealth -= _damageAmount;
	
	if (otherPlayer) 
	{
		otherPlayer->hasLandedAttack = false;
	}

	if (playerHealth <= 0.0f) 
	{
		playerHealth = 0.0f;
	}
}

// This is being called in tick FOR NOW. Go back and change it to when the player is grounded to allow for proper cross-ups!
// Called once per frame.
void ADT_FightingTemplateCharacter::Tick(float deltaTime) 
{
	Super::Tick(deltaTime);

	if (otherPlayer) 
	{
		if (auto characterMovement = GetCharacterMovement()) 
		{
			if (auto enemyMovement = otherPlayer->GetCharacterMovement()) 
			{
				if (enemyMovement->GetActorLocation().Y >= characterMovement->GetActorLocation().Y) 
				{
					if (isFlipped) 
					{
						if (auto mesh = GetCapsuleComponent()->GetChildComponent(1)) 
						{
							transform = mesh->GetRelativeTransform();
							scale = transform.GetScale3D();
							scale.Y = -1;
							transform.SetScale3D(scale);
							mesh->SetRelativeTransform(transform);
						}
						isFlipped = false;
					}
				}
				else 
				{
					if (!isFlipped)
					{
						if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
						{
							transform = mesh->GetRelativeTransform();
							scale = transform.GetScale3D();
							scale.Y = 1;
							transform.SetScale3D(scale);
							mesh->SetRelativeTransform(transform);
						}
						isFlipped = true;
					}
				}

			}
		}
	}
}
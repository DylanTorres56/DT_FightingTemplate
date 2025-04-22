// Copyright Epic Games, Inc. All Rights Reserved.

#include "DT_FightingTemplateCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DT_FightingTemplateGameMode.h"

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
	characterState = ECharacterState::VE_Default;
	transform = FTransform();
	scale = FVector(0.0f, 0.0f, 0.0f);
	attackA_Used = false;
	attackB_Used = false;
	attackC_Used = false;
	attackD_Used = false;
	stunTime = 0.0f;
	playerHealth = 1.00f;
	maxDistanceApart = 800.0f;
	isFacingRight = false;
	hasLandedAttack = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADT_FightingTemplateCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (auto gameMode = Cast<ADT_FightingTemplateGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (gameMode->leftSidePlayer == this) 
		{
			UE_LOG(LogTemp, Warning, TEXT("The left side player has bound their controls."));
			// set up gameplay key bindings
			PlayerInputComponent->BindAction("P1_Jump", IE_Pressed, this, &ADT_FightingTemplateCharacter::Jump);
			PlayerInputComponent->BindAction("P1_Jump", IE_Released, this, &ADT_FightingTemplateCharacter::StopJumping);
			PlayerInputComponent->BindAction("P1_Crouch", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartCrouching);
			PlayerInputComponent->BindAction("P1_Crouch", IE_Released, this, &ADT_FightingTemplateCharacter::StopCrouching);

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
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The right side player has bound their controls."));
			// set up gameplay key bindings
			PlayerInputComponent->BindAction("P2_Jump", IE_Pressed, this, &ADT_FightingTemplateCharacter::Jump);
			PlayerInputComponent->BindAction("P2_Jump", IE_Released, this, &ADT_FightingTemplateCharacter::StopJumping);
			PlayerInputComponent->BindAction("P2_Crouch", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartCrouching);
			PlayerInputComponent->BindAction("P2_Crouch", IE_Released, this, &ADT_FightingTemplateCharacter::StopCrouching);

			PlayerInputComponent->BindAxis("P2_MoveRight", this, &ADT_FightingTemplateCharacter::MoveRight);

			PlayerInputComponent->BindAction("P2_AttackA", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackA);
			// PlayerInputComponent->BindAction("P2_AttackA", IE_Released, this, &ADT_FightingTemplateCharacter::StopAttackA);
			PlayerInputComponent->BindAction("P2_AttackB", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackB);
			// PlayerInputComponent->BindAction("P2_AttackB", IE_Released, this, &ADT_FightingTemplateCharacter::StopAttackB);
			PlayerInputComponent->BindAction("P2_AttackC", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackC);
			// PlayerInputComponent->BindAction("P2_AttackC", IE_Released, this, &ADT_FightingTemplateCharacter::StopAttackC);
			PlayerInputComponent->BindAction("P2_AttackD", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackD);
			// PlayerInputComponent->BindAction("P2_AttackD", IE_Released, this, &ADT_FightingTemplateCharacter::StopAttackD);

			PlayerInputComponent->BindTouch(IE_Pressed, this, &ADT_FightingTemplateCharacter::TouchStarted);
			PlayerInputComponent->BindTouch(IE_Released, this, &ADT_FightingTemplateCharacter::TouchStopped);
		}
	}
	
}

void ADT_FightingTemplateCharacter::Jump()
{
	ACharacter::Jump();
	characterState = ECharacterState::VE_Jumping;
}

void ADT_FightingTemplateCharacter::StopJumping()
{
	ACharacter::StopJumping();
}

void ADT_FightingTemplateCharacter::Landed(const FHitResult& Hit) 
{
	ACharacter::Landed(Hit);
	characterState = ECharacterState::VE_Default;
}

void ADT_FightingTemplateCharacter::StartCrouching()
{
	isCrouching = true;
	characterState = ECharacterState::VE_Crouching;
}

void ADT_FightingTemplateCharacter::StopCrouching()
{
	isCrouching = false;
	characterState = ECharacterState::VE_Default;
}

void ADT_FightingTemplateCharacter::StartBlocking()
{
	characterState = ECharacterState::VE_Blocking;
}

void ADT_FightingTemplateCharacter::StopBlocking()
{
	characterState = ECharacterState::VE_Default;
}

void ADT_FightingTemplateCharacter::MoveRight(float Value)
{
	if(canMove && characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_Blocking)
	{

		// UE_LOG(LogTemp, Warning, TEXT("The directional value is: %f"), Value);
		
		if (characterState != ECharacterState::VE_Jumping)
		{		
			if (Value > 0.20f) 
			{
				characterState = ECharacterState::VE_MovingRight;
			}
			else if (Value < -0.20f)
			{
				characterState = ECharacterState::VE_MovingLeft;
			}
			else 
			{
				characterState = ECharacterState::VE_Default;
			}
		}

		float currentDistanceApart = abs(otherPlayer->GetActorLocation().Y - GetActorLocation().Y);
	
		if(currentDistanceApart >= maxDistanceApart)
		{
			if ((currentDistanceApart + Value < currentDistanceApart && !isFacingRight) || (currentDistanceApart - Value < currentDistanceApart && isFacingRight))
				{
					// add movement in that direction
					AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
				}
		}
		else 
		{
			// add movement in that direction
			AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
		}
	}

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
	MoveRight(_value);
}

void ADT_FightingTemplateCharacter::ProxHitboxCollision() 
{
	if ((characterState == ECharacterState::VE_MovingLeft && isFacingRight) || (characterState == ECharacterState::VE_MovingRight && !isFacingRight))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is blocking!"));
		characterState = ECharacterState::VE_Blocking;
	}
}

void ADT_FightingTemplateCharacter::TakeDamage(float _damageAmount, float _hitstunTime, float _blockstunTime, float _pushbackAmount)
{
	if (characterState != ECharacterState::VE_Blocking) 
	{
		UE_LOG(LogTemp, Warning, TEXT("We are taking damage for %f points."), _damageAmount);
		playerHealth -= _damageAmount;
	
		stunTime = _hitstunTime;

		if (stunTime > 0.0f)
		{
			characterState = ECharacterState::VE_Stunned;
			BeginStun();
		}

		if (otherPlayer) 
		{
			otherPlayer->hasLandedAttack = true; 
			otherPlayer->PerformPushback(_pushbackAmount, false);
		}
		
		otherPlayer->PerformPushback(_pushbackAmount, false);

		if (playerHealth <= 0.0f) 
		{
			playerHealth = 0.0f;
		}
	}
	else 
	{
		float reducedDamage = _damageAmount * 0.5f;
		UE_LOG(LogTemp, Warning, TEXT("We are taking chip damage for %f points."), reducedDamage);
		playerHealth -= reducedDamage;

		stunTime = _blockstunTime;
		if (stunTime >  0.0f)
		{
			BeginStun();
		}
		else 
		{
			characterState = ECharacterState::VE_Default;
		}

		if (otherPlayer) 
		{
			otherPlayer->hasLandedAttack = false;
			otherPlayer->PerformPushback(_pushbackAmount, false);
		}

		otherPlayer->PerformPushback(_pushbackAmount, true);
	}
	
}

void ADT_FightingTemplateCharacter::BeginStun() 
{
	canMove = false;
	GetWorld()->GetTimerManager().SetTimer(stunTimerHandle, this, &ADT_FightingTemplateCharacter::EndStun, stunTime, false);
}

void ADT_FightingTemplateCharacter::EndStun()
{
	characterState = ECharacterState::VE_Default;
	canMove = true;
}

void ADT_FightingTemplateCharacter::PerformPushback(float _pushbackAmount, bool _hasBlocked)
{
	if (_hasBlocked) 
	{
		if (isFacingRight) 
		{
			LaunchCharacter(FVector(0.0f, -_pushbackAmount * 2.0f, 0.0f), false, false);
		}
		else 
		{
			LaunchCharacter(FVector(0.0f, _pushbackAmount * 2.0f, 0.0f), false, false);
		}
	}
	else 
	{
		if (isFacingRight)
		{
			LaunchCharacter(FVector(0.0f, -_pushbackAmount, 0.0f), false, false);
		}
		else
		{
			LaunchCharacter(FVector(0.0f, _pushbackAmount, 0.0f), false, false);
		}
	}
}

// This is being called in tick FOR NOW. Go back and change it to when the player is grounded to allow for proper cross-ups!
// Called once per frame.
void ADT_FightingTemplateCharacter::Tick(float deltaTime) 
{
	Super::Tick(deltaTime);

	if (characterState != ECharacterState::VE_Jumping) 
	{
	
		if (otherPlayer) 
		{
			if (auto characterMovement = GetCharacterMovement()) 
			{
				if (auto enemyMovement = otherPlayer->GetCharacterMovement()) 
				{
					if (enemyMovement->GetActorLocation().Y >= characterMovement->GetActorLocation().Y)
					{
						if (!isFacingRight) 
						{
							if (auto mesh = GetCapsuleComponent()->GetChildComponent(1)) 
							{
								transform = mesh->GetRelativeTransform();
								scale = transform.GetScale3D();
								scale.Y = 1;
								transform.SetScale3D(scale);
								mesh->SetRelativeTransform(transform);
							}
							isFacingRight = true;
						}
					}
					else 
					{
						if (isFacingRight)
						{
							if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
							{
								transform = mesh->GetRelativeTransform();
								scale = transform.GetScale3D();
								scale.Y = -1;
								transform.SetScale3D(scale);
								mesh->SetRelativeTransform(transform);
							}
							isFacingRight = false;
						}
					}
	
				}
			}
		}
	}
}
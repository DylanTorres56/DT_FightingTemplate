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
	canUseExAttack = true;
	attackTestEX_Used = false;
	superUsed = false;
	
	removeInputFromBufferTime = 1.4f;

	// Create and assign commands.
	characterCommands.SetNum(2);

	characterCommands[0].name = "236A";
	characterCommands[0].inputs.Add("U");
	characterCommands[0].inputs.Add("O");
	characterCommands[0].inputs.Add("U");
	characterCommands[0].hasUsedCommand = false;

	characterCommands[1].name = "236C";
	characterCommands[1].inputs.Add("I");
	characterCommands[1].inputs.Add("P");
	characterCommands[1].inputs.Add("O");
	characterCommands[1].hasUsedCommand = false;

	stunTime = 0.0f;
	gravityScale = GetCharacterMovement()->GravityScale;
	superMeterAmount = 0.0f;
	playerMaxHP = 1.00f;
	playerHealth = playerMaxHP;
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

			PlayerInputComponent->BindAction("P1_EX-Test", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackTestEX);
			PlayerInputComponent->BindAction("P1_Super-Test", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartSuperAttack);

			// This assumes LSP's inputs are filtered to controls they can use.
			// PlayerInputComponent->BindAction("P1_AddToInputBuffer", IE_Pressed, this, &ADT_FightingTemplateCharacter::AddInputToInputBuffer);
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

			PlayerInputComponent->BindAction("P2_EX-Test", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartAttackTestEX);
			PlayerInputComponent->BindAction("P2_Super-Test", IE_Pressed, this, &ADT_FightingTemplateCharacter::StartSuperAttack);

			// This assumes RSP's inputs are filtered to controls they can use.
			// PlayerInputComponent->BindAction("P2_AddToInputBuffer", IE_Pressed, this, &ADT_FightingTemplateCharacter::AddInputToInputBuffer);
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
	// ACharacter::Landed(Hit);
	if (characterState == ECharacterState::VE_Launched || characterState == ECharacterState::VE_Jumping) 
	{
		GetCharacterMovement()->GravityScale = gravityScale;
		characterState = ECharacterState::VE_Default;
	}
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
		
		if (characterState != ECharacterState::VE_Jumping && characterState != ECharacterState::VE_Launched)
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

void ADT_FightingTemplateCharacter::StartAttackTestEX() 
{
	UE_LOG(LogTemp, Warning, TEXT("Test EX Attack called!"));
	if (superMeterAmount >= .25f && canUseExAttack) 
	{
		UE_LOG(LogTemp, Warning, TEXT("25 Super Meter burned for an EX Attack!"));
		attackTestEX_Used = true;
		superMeterAmount -= .25f;
		ApplyMeterBurnMat();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough Super Meter for this move!"));
	}

	if (superMeterAmount < 0.0f)
	{
		superMeterAmount = 0.0f;
	}
}

void ADT_FightingTemplateCharacter::StartSuperAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Super Attack used!"));
	if (superMeterAmount >= .75f) 
	{
		UE_LOG(LogTemp, Warning, TEXT("75 Super Meter burned for a Super Attack!"));
		superUsed = true;
		superMeterAmount -= .75f;
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough Super Meter for this move!"));
	}

	if (superMeterAmount < 0.0f)
	{
		superMeterAmount = 0.0f;
	}
}

void ADT_FightingTemplateCharacter::AddInputToBuffer(FInputInfo _inputInfo)
{
	inputBuffer.Add(_inputInfo);
	CheckInputBufferForCommand();
	// GetWorld()->GetTimerManager().SetTimer(inputBufferTimerHandle, this, &ADTFightingTemplateCharacter::RemoveInputFromInputBuffer, removeInputFromBufferTime, false);
}

void ADT_FightingTemplateCharacter::RemoveInputFromBuffer()
{

}

void ADT_FightingTemplateCharacter::CheckInputBufferForCommand()
{
	int correctSequenceCounter = 0;

	for (auto currentCommand : characterCommands)
	{	
		for (int commandInput = 0; commandInput < currentCommand.inputs.Num(); ++commandInput) 
		{
			for (int i = 0; i < inputBuffer.Num(); ++i) 
			{
				if (i + correctSequenceCounter < inputBuffer.Num())
				{
					if (inputBuffer[i + correctSequenceCounter].inputName.Compare(currentCommand.inputs[commandInput]) == 0)
					{
						UE_LOG(LogTemp, Warning, TEXT("The player has added another input to the command buffer!"));
						++correctSequenceCounter;
	
						if (correctSequenceCounter == currentCommand.inputs.Num())
						{
							StartCommand(currentCommand.name);
						}
	
						break;
	
					}
					else 
					{
						UE_LOG(LogTemp, Warning, TEXT("The player has broken sequence."));
						correctSequenceCounter = 0;
					}
				}
				else 
				{
					UE_LOG(LogTemp, Warning, TEXT("The player is not finished with the sequence yet."));
					correctSequenceCounter = 0;
				}
			}
		}
	}
}

void ADT_FightingTemplateCharacter::StartCommand(FString _commandName) 
{
	for(int currentCommand = 0; currentCommand < characterCommands.Num(); ++currentCommand)	
	{
		if (_commandName.Compare(characterCommands[currentCommand].name) == 0) 
		{
			UE_LOG(LogTemp, Warning, TEXT("The player is using the command: %s."), *_commandName);
			characterCommands[currentCommand].hasUsedCommand = true;
		}
	}
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

void ADT_FightingTemplateCharacter::P2_StartAttackTestEX()
{
	// UE_LOG(LogTemp, Warning, TEXT("Test EX Attack called!"));
	StartAttackTestEX();
}

void ADT_FightingTemplateCharacter::P2_StartSuperAttack()
{
	// UE_LOG(LogTemp, Warning, TEXT("Super Attack called!"));
	StartSuperAttack();
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

void ADT_FightingTemplateCharacter::TakeDamage(float _damageAmount, float _hitstunTime, float _blockstunTime, float _pushbackAmount, float _launchAmount)
{
	if (characterState != ECharacterState::VE_Blocking) 
	{
		UE_LOG(LogTemp, Warning, TEXT("We are taking damage for %f points."), _damageAmount);
		playerHealth -= _damageAmount;
		superMeterAmount += _damageAmount * 0.85f;

		stunTime = _hitstunTime;

		if (stunTime > 0.0f)
		{
			characterState = ECharacterState::VE_Stunned;
			BeginStun();
		}

		if (otherPlayer) 
		{
			otherPlayer->hasLandedAttack = true; 
			otherPlayer->PerformPushback(_pushbackAmount, 0.0f, false);

			if (!otherPlayer->attackTestEX_Used) // If the opponent ISN'T using a Test EX, they gain less meter on hit.
			{
				otherPlayer->superMeterAmount += _damageAmount * 0.3f;
			}
		}
		
		PerformPushback(_pushbackAmount, _launchAmount, false);

		if (playerHealth <= 0.0f) 
		{
			playerHealth = 0.0f;
		}
		else if (playerHealth > 0.0f && playerHealth < (playerMaxHP * 0.5f)) 
		{
			ChangeToDamagedMat();
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
			otherPlayer->PerformPushback(_pushbackAmount, 0.0f, false);
		}

		PerformPushback(_pushbackAmount, _launchAmount, true);
	}
	
}

void ADT_FightingTemplateCharacter::BeginStun() 
{
	canMove = false;
	GetWorld()->GetTimerManager().SetTimer(stunTimerHandle, this, &ADT_FightingTemplateCharacter::EndStun, stunTime, false);
}

void ADT_FightingTemplateCharacter::EndStun()
{
	if (characterState != ECharacterState::VE_Launched)
	{
		characterState = ECharacterState::VE_Default;
	}
	canMove = true;
}

void ADT_FightingTemplateCharacter::PerformPushback(float _pushbackAmount, float _launchAmount, bool _hasBlocked)
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
		if (_launchAmount > 0.0f) 
		{
			GetCharacterMovement()->GravityScale *= 0.7f;
			characterState = ECharacterState::VE_Launched;
		}

		if (isFacingRight)
		{
			LaunchCharacter(FVector(0.0f, -_pushbackAmount, _launchAmount), false, false);
		}
		else
		{
			LaunchCharacter(FVector(0.0f, _pushbackAmount, _launchAmount), false, false);
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
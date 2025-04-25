// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DT_FightingTemplateCharacter.generated.h"
UENUM(BlueprintType)
enum class ECharacterState : uint8 
{
	VE_Default			UMETA(DisplayName = "NEUTRAL"),
	VE_MovingRight		UMETA(DisplayName = "MOVING_RIGHT"),
	VE_RunningRight		UMETA(DisplayName = "RUNNING_RIGHT"),
	VE_MovingLeft		UMETA(DisplayName = "MOVING_LEFT"),
	VE_RunningLeft		UMETA(DisplayName = "RUNNING_LEFT"),
	VE_RollingForward	UMETA(DisplayName = "ROLLING_FORWARD"),
	VE_RollingBackward	UMETA(DisplayName = "ROLLING_BACKWARD"),
	VE_Crouching		UMETA(DisplayName = "CROUCHING"),
	VE_Jumping			UMETA(DisplayName = "JUMPING"),
	VE_Stunned			UMETA(DisplayName = "STUNNED"),
	VE_Blocking			UMETA(DisplayName = "BLOCKING"),
	VE_Launched			UMETA(DisplayName = "LAUNCHED")
};

USTRUCT(BlueprintType)
struct FInputInfo
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FString inputName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float timeStamp;
};

USTRUCT(BlueprintType)
struct FCommand
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FString> inputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool hasUsedCommand;
};

UCLASS(config=Game)
class ADT_FightingTemplateCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	// Camera boom positioning the camera beside the character 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom; */

	void StartAttackA();
	void StartAttackB();
	void StartAttackC();
	void StartAttackD();
	void StartAttackTestEX();
	void StartSuperAttack();

	// When in Keyboard-Only mode, use these functions to control P2.
	UFUNCTION(BlueprintCallable)
	void P2_StartAttackA();

	UFUNCTION(BlueprintCallable)
	void P2_StartAttackB();

	UFUNCTION(BlueprintCallable)
	void P2_StartAttackC();

	UFUNCTION(BlueprintCallable)
	void P2_StartAttackD();

	UFUNCTION(BlueprintCallable)
	void P2_StartAttackTestEX();

	UFUNCTION(BlueprintCallable)
	void P2_StartSuperAttack();

	UFUNCTION(BlueprintCallable)
	void P2_Jump();

	UFUNCTION(BlueprintCallable)
	void P2_StopJumping();
	
	UFUNCTION(BlueprintCallable)
	void P2_MoveRight(float _value);

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	// Called every frame
	virtual void Tick(float deltaTime) override;

	// Override the jump functions in ACharacter/APawn to refine jump and landing logic.
	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void Landed(const FHitResult& Hit) override;

	// Character begins crouching.
	UFUNCTION(BlueprintCallable)
	void StartCrouching();

	// Character ends crouching.
	UFUNCTION(BlueprintCallable)
	void StopCrouching();

	// The hurtbox attached to the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	AActor* hurtbox;

	// Damage the player!
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float _damageAmount, float _hitstunTime, float _blockstunTime, float _pushbackAmount, float _launchAmount);

	// Determine the distance to push back a struck opponent.
	UFUNCTION(BlueprintCallable)
	void PerformPushback(float _pushbackAmount, float _launchAmount, bool _hasBlocked);

	// Change the player's mat when damaged!
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeToDamagedMat();

	// Change the player's mat when damaged!
	UFUNCTION(BlueprintImplementableEvent)
	void AddInputIconToHistory(int _iconIndex, bool _shouldAddInput = true);

	// Stun state begins!
	void BeginStun();

	// Stun state ends!
	void EndStun();

	// Add inputs to the input buffer.
	UFUNCTION(BlueprintCallable)
	void AddInputToBuffer(FInputInfo _inputInfo);

	// Remove inputs from the input buffer.
	UFUNCTION(BlueprintCallable)
	void RemoveInputFromBuffer();

	// Check the input buffer to detect if a player has performed a command move.
	UFUNCTION(BlueprintCallable)
	void CheckInputBufferForCommand();

	// Make the character begin using a command based off its name.
	UFUNCTION(BlueprintCallable)
	void StartCommand(FString _commandName);

	// Has the player released an axis input?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool hasReleasedAxisInput;

	// The array of inputs the player controlling this character has performed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FInputInfo> inputBuffer;

	// The array of commands to be used when the correct inputs are performed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FCommand> characterCommands;

	// Has the player collided with a prox hitbox?
	UFUNCTION(BlueprintCallable)
	void ProxHitboxCollision();

	// Begin blocking!
	UFUNCTION(BlueprintCallable)
	void StartBlocking();

	// End blocking!
	UFUNCTION(BlueprintCallable)
	void StopBlocking();

	// The amount of time a character is stunned for.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float stunTime;

	// The timer handle for all stuns (hits, blocks and general stuns).
	FTimerHandle stunTimerHandle;

	// The timer handle for all stuns (hits, blocks and general stuns).
	FTimerHandle inputBufferTimerHandle;

	// The amount of time before inputs are removed from the buffer.
	float removeInputFromBufferTime;

	// A reference to the other player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ADT_FightingTemplateCharacter* otherPlayer;

	// The direction the character is moving OR the direction the player is holding down (could also be neutral).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	ECharacterState characterState;

	// Is the player currently able to move?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool canMove;

	// Is the player currently crouching?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool isCrouching;

	// The scale of gravity on a launched player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float gravityScale;

	// Has the player pressed A (Light Punch)?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool attackA_Used;

	// Has the player pressed B (Light Kick)?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool attackB_Used;

	// Has the player pressed C (Heavy Punch)?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool attackC_Used;

	// Has the player pressed D (Heavy Kick)?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool attackD_Used;

	// Is the player able to use an EX attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool canUseExAttack;

	// Has the player pressed the Test EX attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool attackTestEX_Used;

	// Has the player pressed the Super Attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool superUsed;

	// The amount of Super Meter the player currently has.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Super Meter")
	float superMeterAmount;

	// Update material to meter burned color!
	UFUNCTION(BlueprintImplementableEvent)
	void ApplyMeterBurnMat();

	// The max amount of health the player can have.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float playerMaxHP;

	// The amount of health the player currently has.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float playerHealth;

	// The maximum amount of distance that can separate players.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float maxDistanceApart;

	// Is the character's model currently flipped?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	bool isFacingRight;

	// Transform of the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FTransform transform;

	// Scale vector of the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FVector scale;

	// Has the player's last attack connected?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool hasLandedAttack;

	// Does this device support multiple players at once?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	bool doesDeviceSupportMultiplePlayers;


public:
	ADT_FightingTemplateCharacter();

	/* Returns SideViewCameraComponent subobject
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	// Returns CameraBoom subobject 
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; } */
};

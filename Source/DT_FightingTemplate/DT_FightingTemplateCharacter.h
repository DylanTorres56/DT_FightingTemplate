// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DT_FightingTemplateCharacter.generated.h"
UENUM(BlueprintType)
enum class EDirectionalInput : uint8 
{
	VE_Default			UMETA(DisplayName = "NEUTRAL"),
	VE_MovingRight		UMETA(DisplayName = "MOVING_RIGHT"),
	VE_MovingLeft		UMETA(DisplayName = "MOVING_LEFT")
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

	// The hurtbox attached to the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	AActor* hurtbox;

	// Damage the player!
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float _damageAmount);

	// A reference to the other player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ADT_FightingTemplateCharacter* otherPlayer;

	// The direction the character is moving OR the direction the player is holding down (could also be neutral).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EDirectionalInput directionalInput;

	// Is the player currently able to move?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool canMove;

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

	// The amount of health the player currently has.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float playerHealth;

	// The maximum amount of distance that can separate players.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float maxDistanceApart;

	// Is the character's model currently flipped?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	bool isFlipped;

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

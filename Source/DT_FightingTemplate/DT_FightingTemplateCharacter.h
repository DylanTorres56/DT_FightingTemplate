// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DT_FightingTemplateCharacter.generated.h"

UCLASS(config=Game)
class ADT_FightingTemplateCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	void StartAttackA();
	void StartAttackB();
	void StartAttackC();
	void StartAttackD();

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

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks");
	//ECharacterClass characterClass;

	// Damage the player!
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float _damageAmount);

	// Has the player pressed A (light jab)?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks");
	bool attackA_Used;

	// The amount of health the player currently has.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float playerHealth;

public:
	ADT_FightingTemplateCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

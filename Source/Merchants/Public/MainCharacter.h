// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MainCharacter.generated.h"

class AWeapon;
class AMainPlayerController;

UCLASS(config = Game)
class AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AMainCharacter();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Interact Input Action */	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Interact, meta = (AllowPrivateAccess = "true"))
	int32 MaxInteractDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interact, meta = (AllowPrivateAccess = "true"))
	class AInteractable* Interactable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;

private:

	FTimerHandle InteractableCheckTimer;

	void CheckInteractable();

	AMainPlayerController* MainPlayerController;

	AWeapon* CurrentWeapon;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Interact();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	virtual void SetupWeapon();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
};


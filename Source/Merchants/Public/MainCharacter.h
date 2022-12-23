// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Items/Item.h"
#include "MainCharacter.generated.h"

class AWeapon;
class AMainPlayerController;
class AInteractable;
class UCombatComponent;
class ACombatCharacter;
class UInventoryComponent;

UENUM(BlueprintType)
enum class ECombatMode : uint8
{
	ECM_Unarmed UMETA(DisplayName = "Unarmed"),
	ECM_OneHanded UMETA(DisplayName = "OneHanded"),
	ECM_TwoHanded UMETA(DisplayName = "TwoHanded"),

	ECM_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS(config = Game)
class AMainCharacter : public ACharacter, public IItemsHolder
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

	/** Gell All Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* GetAllAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToogleInventoryAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Interact, meta = (AllowPrivateAccess = "true"))
	int32 MaxInteractDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interact, meta = (AllowPrivateAccess = "true"))
	AInteractable* Interactable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|Combat", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* OneHandedCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ACombatCharacter* CombatTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	bool bRotateToTarget;

private:

	AMainPlayerController* MainPlayerController;
	
	UPROPERTY(Replicated)
	AWeapon* CurrentWeapon;
	
	FTimerHandle InteractableCheckTimer;

	void CheckInteractable();

protected:

	ECombatMode CombatMode;
	TMap<ECombatMode, UCombatComponent*> CombatComponents;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Interact();

	void Attack();
	void StopAttack();

public:

	// ItemsHolder
	virtual TArray<FItem> GetItems() const override;
	virtual bool CanAdd(const FName ItemId, const int32 Quantity) const override;
	virtual bool CanRemove(const FName ItemId, const int32 Quantity) const override;
	virtual int32 Add(const FName ItemId, const int32 Quantity) override;
	virtual int32 Remove(const FName ItemId, const int32 Quantity) override;
	//------------------

	void ToogleInventory();
	void OpenBag();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void TakeItem(const TScriptInterface<IItemsHolder>& Holder, const FName ItemId, const int32 Quantity);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void GiveItem(const TScriptInterface<IItemsHolder>& Holder, const FName ItemId, const int32 Quantity);

	UFUNCTION(Server, Reliable)
	void GetAll();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupWeapon();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	FORCEINLINE UCombatComponent* GetCurrentCombatComponent() const { return CombatComponents[CombatMode]; }
};


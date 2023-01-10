// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Items/Item.h"
#include "Navigation/CrowdAgentInterface.h"
#include "CombatCharacter.h"
#include "MainCharacter.generated.h"

class AWeapon;
class AMainPlayerController;
class AInteractable;
class UCombatComponent;
class UInventoryComponent;
class UAttributesComponent;
class UDamageType;
class ANPCharacter;

UENUM(BlueprintType)
enum class ECombatMode : uint8
{
	ECM_Unarmed UMETA(DisplayName = "Unarmed"),
	ECM_OneHanded UMETA(DisplayName = "OneHanded"),
	ECM_TwoHanded UMETA(DisplayName = "TwoHanded"),

	ECM_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS(config = Game)
class AMainCharacter : public ACombatCharacter, public IItemsHolder, public ICrowdAgentInterface
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

	/** Punch Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* RightPunchSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FName RightPunchSphereSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* LeftPunchSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FName LeftPunchSphereSocketName;

	/***/

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

	/** Zoom Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ZoomAction;

	/** Toogle Inventory Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToogleInventoryAction;

	/** Toogle Minimap Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToogleMinimapAction;

	/** Force Mouse Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShowMouseAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Interact, meta = (AllowPrivateAccess = "true"))
	int32 MaxInteractDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interact, meta = (AllowPrivateAccess = "true"))
	AInteractable* Interactable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Components", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* OneHandedCombatComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Components", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* UnarmedCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ACombatCharacter* CombatTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UAttributesComponent* AttributesComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Zoom", meta = (AllowPrivateAccess = "true"))
	float CameraMinZoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Zoom", meta = (AllowPrivateAccess = "true"))
	float CameraMaxZoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Zoom", meta = (AllowPrivateAccess = "true"))
	float ZoomSpeed;

	float ZoomTarget;

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

	/** Called for looking input */
	void Zoom(const FInputActionValue& Value);

	/** Force showing the mouse */
	void ShowMouse();
	void StopShowingMouse();

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
	void ToogleMinimap();
	void OpenBag();
	void CloseBag();
	void OpenDialog(ANPCharacter* NPC);

	UFUNCTION()
	void OnInteractibleDestroyed(AActor* DestroyedActor);

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void TakeItem(const TScriptInterface<IItemsHolder>& Holder, const FName ItemId, const int32 Quantity);

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void GiveItem(const TScriptInterface<IItemsHolder>& Holder, const FName ItemId, const int32 Quantity);

	UFUNCTION(Server, Unreliable)
	void GetAll();

	UFUNCTION(Server, Unreliable)
	void SetLocationAndRotation(FVector NewLocation, FRotator NewRotation);

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void BuyItem(FName ItemId, int32 Quantity);

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void SellItem(FName ItemId, int32 Quantity);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	// Called before destroying the object
	virtual void BeginDestroy() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupWeapon();

public:
	// ICrowdAgentInterface BEGIN
	virtual FVector GetCrowdAgentLocation() const override;
	virtual FVector GetCrowdAgentVelocity() const override;
	virtual void GetCrowdAgentCollisions(float& CylinderRadius, float& CylinderHalfHeight) const override;
	virtual float GetCrowdAgentMaxSpeed() const override;
	virtual int32 GetCrowdAgentAvoidanceGroup() const override;
	virtual int32 GetCrowdAgentGroupsToAvoid() const override;
	virtual int32 GetCrowdAgentGroupsToIgnore() const override;
	// ICrowdAgentInterface END

private:
	FCombatStats* CombatStats;

public:
	// implement CombatCharacter
	virtual float GetMaxHealth() const override;
	virtual float GetHealth() const override;
	virtual FText GetCharacterName() const override;
	virtual ECharacterType GetCharacterType() const override;
	virtual FCombatStats* GetCombatStats() const override;

public:

	void ActivateLeftPunchCollistion();
	void DeactivateLeftPunchCollistion();
	void ActivateRightPunchCollision();
	void DeactivateRightPunchCollision();

	UFUNCTION()
	void PunchOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Unreliable)
	void DealDamage(ACombatCharacter* OtherActor);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	FORCEINLINE UCombatComponent* GetCurrentCombatComponent() const { return CombatComponents[CombatMode]; }
};


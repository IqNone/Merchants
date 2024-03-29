// Copyright Epic Games, Inc. All Rights Reserved.

#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interactables/Interactable.h"
#include "MainPlayerController.h"
#include "Weapons/Weapon.h"
#include "Components/Combat/CombatComponent.h"
#include "CombatCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InventoryComponent.h"
#include "Components/AttributesComponent.h"
#include "Items/Item.h"
#include "Net/UnrealNetwork.h"
#include "Gameplay/GameplayUtils.h"
#include "NPCs/NPCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AMerchantsCharacter

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Camera arm length clamp
	CameraMinZoom = 100.f;
	CameraMaxZoom = 1200.f;
	ZoomSpeed = 150.f;
	ZoomTarget = 400.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Right Punch
	RightPunchSphereSocketName = "RightPunchSphere";
	RightPunchSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightPunchSphere"));
	RightPunchSphere->SetupAttachment(GetMesh(), RightPunchSphereSocketName);
	RightPunchSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightPunchSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightPunchSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightPunchSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// Right Punch
	LeftPunchSphereSocketName = "LeftPunchSphere";
	LeftPunchSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftPunchSphere"));
	LeftPunchSphere->SetupAttachment(GetMesh(), LeftPunchSphereSocketName);
	LeftPunchSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftPunchSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftPunchSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftPunchSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	MaxInteractDistance = 200;

	WeaponSocketName = "WeaponSocket";

	CombatMode = ECombatMode::ECM_Unarmed;
	UnarmedCombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("UnarmedCombatComponent"));
	OneHandedCombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("OneHandedCombatComponent"));

	CombatComponents.Add(ECombatMode::ECM_Unarmed, UnarmedCombatComponent);
	CombatComponents.Add(ECombatMode::ECM_OneHanded, OneHandedCombatComponent);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));	

	// Don't create the attributes component on "other" clients
	if (GetLocalRole() != ROLE_SimulatedProxy)
	{
		AttributesComponent = CreateDefaultSubobject<UAttributesComponent>("AttributesComponent");
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		// Only on server
		CombatStats = new FCombatStats();
	}
}

void AMainCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(Controller))
	{
		MainPlayerController = PlayerController;
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	GetWorldTimerManager().SetTimer(InteractableCheckTimer, this, &AMainCharacter::CheckInteractable, .5f, true);

	LeftPunchSphere->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::PunchOverlapBegin);
	RightPunchSphere->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::PunchOverlapBegin);

	//SetupWeapon();
}

void AMainCharacter::BeginDestroy()
{
	Super::BeginDestroy();

	if (CombatStats)
	{
		delete CombatStats;
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotateToTarget && CombatTarget && CombatTarget->GetHealth() > 0)
	{
		FRotator LookAtYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CombatTarget->GetActorLocation());
		LookAtYaw.Pitch = 0;
		LookAtYaw.Roll = 0;
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, 15.f);

		SetActorRotation(InterpRotation);
	}

	if (ZoomTarget != CameraBoom->TargetArmLength)
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, ZoomTarget, DeltaTime, 10.f);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMainCharacter::Interact);

		//Get All Items
		EnhancedInputComponent->BindAction(GetAllAction, ETriggerEvent::Triggered, this, &AMainCharacter::GetAll);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);

		//Zooming
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AMainCharacter::Zoom);

		//Attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMainCharacter::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AMainCharacter::StopAttack);

		//Toogle Inventory
		EnhancedInputComponent->BindAction(ToogleInventoryAction, ETriggerEvent::Triggered, this, &AMainCharacter::ToogleInventory);

		//Toogle Minimap
		EnhancedInputComponent->BindAction(ToogleMinimapAction, ETriggerEvent::Triggered, this, &AMainCharacter::ToogleMinimap);

		//Force show mouse
		EnhancedInputComponent->BindAction(ShowMouseAction, ETriggerEvent::Triggered, this, &AMainCharacter::ShowMouse);
		EnhancedInputComponent->BindAction(ShowMouseAction, ETriggerEvent::Completed, this, &AMainCharacter::StopShowingMouse);
	}

}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}

	if (MainPlayerController)
	{
		MainPlayerController->CloseBag();
	}
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMainCharacter::Zoom(const FInputActionValue& Value)
{
	ZoomTarget = FMath::Clamp(ZoomTarget - ZoomSpeed * Value.Get<float>(), CameraMinZoom, CameraMaxZoom);
}

void AMainCharacter::ToogleInventory()
{
	if (MainPlayerController)
	{		
		MainPlayerController->ToogleInventory();
	}
}

void AMainCharacter::ToogleMinimap()
{
	if (MainPlayerController)
	{
		MainPlayerController->ToogleMinimap();
	}
}

void AMainCharacter::OpenBag()
{
	if (MainPlayerController && Interactable)
	{
		MainPlayerController->OpenBag();
		Interactable->OnDestroyed.AddDynamic(this, &AMainCharacter::OnInteractibleDestroyed);
	}
}

void AMainCharacter::CloseBag()
{
	if (MainPlayerController)
	{
		MainPlayerController->CloseBag();
	}
}

void AMainCharacter::OpenDialog(ANPCharacter* NPC)
{
	if (MainPlayerController)
	{
		MainPlayerController->OpenDialog(NPC);
	}
}

void AMainCharacter::OnInteractibleDestroyed(AActor* DestroyedActor)
{	
	if (DestroyedActor == Interactable)
	{
		CloseBag();
	}
}

void AMainCharacter::TakeItem_Implementation(const TScriptInterface<IItemsHolder>& Holder, const FName ItemId, const int32 Quantity)
{
	if (CanAdd(ItemId, Quantity) && Holder && Holder->CanRemove(ItemId, Quantity))
	{
		int32 Removed = Holder->Remove(ItemId, Quantity);
		Add(ItemId, Removed);
	}
}

void AMainCharacter::GiveItem_Implementation(const TScriptInterface<IItemsHolder>& Holder, const FName ItemId, const int32 Quantity)
{
	if (CanRemove(ItemId, Quantity) && Holder && Holder->CanAdd(ItemId, Quantity))
	{
		int32 Removed = Remove(ItemId, Quantity);
		Holder->Add(ItemId, Removed);
	}
}

void AMainCharacter::GetAll_Implementation()
{
	IItemsHolder* Holder = Cast<IItemsHolder>(Interactable);
	if (!Holder)
	{
		return;
	}

	TScriptInterface<IItemsHolder> Target;
	Target.SetInterface(Holder);
	Target.SetObject(Interactable);

	for (FItem Item : Holder->GetItems())
	{
		TakeItem(Target, Item.ItemId, Item.Quantity);
	}	
}

void AMainCharacter::SetLocationAndRotation_Implementation(FVector NewLocation, FRotator NewRotation)
{
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void AMainCharacter::BuyItem_Implementation(FName ItemId, int32 Quantity)
{
	ANPCharacter* NPC = Cast<ANPCharacter>(Interactable->GetOwner());
	
	if (!NPC || (NPC->GetActorLocation() - GetActorLocation()).Length() >= MaxInteractDistance + 100)
	{
		return;
	}

	int32 Price = NPC->GetNPCSellPrice(ItemId);
	if (Price <= 0)
	{
		return;
	}

	if (InventoryComponent->CanAdd(ItemId, Quantity) && InventoryComponent->GetCoins() >= Price * Quantity)
	{
		InventoryComponent->Add(ItemId, Quantity);
		InventoryComponent->RemoveCoins(Price * Quantity);
	}
}

void AMainCharacter::SellItem_Implementation(FName ItemId, int32 Quantity)
{
	ANPCharacter* NPC = Cast<ANPCharacter>(Interactable->GetOwner());
	if (!NPC || (NPC->GetActorLocation() - GetActorLocation()).Length() >= MaxInteractDistance + 100)
	{
		return;
	}

	int32 Price = NPC->GetNPCBuyPrice(ItemId);
	if (Price <= 0)
	{
		return;
	}

	// Sell as much as we can
	int32 SoldQuantity = InventoryComponent->Remove(ItemId, Quantity);
	InventoryComponent->AddCoins(SoldQuantity * Price);
}

void AMainCharacter::CheckInteractable()
{
	FVector Forward = GetFollowCamera()->GetComponentRotation().Vector();
	FVector Start = GetFollowCamera()->GetComponentLocation() + Forward * GetCameraBoom()->TargetArmLength;
	FVector End = Start + Forward * MaxInteractDistance;

	FHitResult Hit;
	FCollisionObjectQueryParams ObjectParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldDynamic);
	FCollisionQueryParams QueryParams;	

	GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectParams, QueryParams);
	
	Interactable = Cast<AInteractable>(Hit.GetActor());
}

void AMainCharacter::Interact()
{
	if (Interactable)
	{
		Interactable->Interact(this);
	}
}

void AMainCharacter::Attack()
{
	bRotateToTarget = true;
	CombatComponents[CombatMode]->Attack();
}

void AMainCharacter::StopAttack()
{
	bRotateToTarget = false;
	CombatComponents[CombatMode]->StopAttack();
}

void AMainCharacter::ShowMouse()
{
	if (MainPlayerController)
	{
		MainPlayerController->ShowMouse();
	}
}

void AMainCharacter::StopShowingMouse()
{
	if (MainPlayerController)
	{
		MainPlayerController->StopShowingMouse();
	}
}

void AMainCharacter::SetupWeapon()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->OnUnequipped();
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	SpawnParams.Owner = this;

	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->OnEquipped();
	}
}

// Start ItemsHolder implementation

TArray<FItem> AMainCharacter::GetItems() const
{
	return InventoryComponent->GetItems();
}

bool AMainCharacter::CanAdd(const FName ItemId, const int32 Quantity) const
{
	return InventoryComponent->CanAdd(ItemId, Quantity);
}

bool AMainCharacter::CanRemove(const FName ItemId, const int32 Quantity) const
{
	return InventoryComponent->CanRemove(ItemId, Quantity);
}

int32 AMainCharacter::Add(const FName ItemId, const int32 Quantity)
{
	return InventoryComponent->Add(ItemId, Quantity);
}

int32 AMainCharacter::Remove(const FName ItemId, const int32 Quantity)
{
	return InventoryComponent->Remove(ItemId, Quantity);
}

// End ItemsHolder implementation

// ICrowdAgentInterface BEGIN
FVector AMainCharacter::GetCrowdAgentLocation() const
{
	return GetCharacterMovement() ? GetCharacterMovement()->GetActorFeetLocation() : FVector::ZeroVector;
}

FVector AMainCharacter::GetCrowdAgentVelocity() const
{
	return GetCharacterMovement() ? GetCharacterMovement()->Velocity : FVector::ZeroVector;
}

void AMainCharacter::GetCrowdAgentCollisions(float& CylinderRadius, float& CylinderHalfHeight) const
{
	if (GetCharacterMovement() && GetCharacterMovement()->UpdatedComponent)
	{
		GetCharacterMovement()->UpdatedComponent->CalcBoundingCylinder(CylinderRadius, CylinderHalfHeight);
	}
}

float AMainCharacter::GetCrowdAgentMaxSpeed() const
{
	return GetCharacterMovement() ? GetCharacterMovement()->GetMaxSpeed() : 0;
}

int32 AMainCharacter::GetCrowdAgentAvoidanceGroup() const
{
	return GetCharacterMovement() ? GetCharacterMovement()->GetAvoidanceGroupMask() : 0;
}

int32 AMainCharacter::GetCrowdAgentGroupsToAvoid() const
{
	return GetCharacterMovement() ? GetCharacterMovement()->GetGroupsToAvoidMask() : 0;
}
int32 AMainCharacter::GetCrowdAgentGroupsToIgnore() const
{
	return GetCharacterMovement() ? GetCharacterMovement()->GetGroupsToIgnoreMask() : 0;
}

// ICrowdAgentInterface END

// CombatCharacter START
float AMainCharacter::GetMaxHealth() const
{
	return 100;
}

float AMainCharacter::GetHealth() const
{
	return 100;
}

FText AMainCharacter::GetCharacterName() const
{
	return FText();
}

ECharacterType AMainCharacter::GetCharacterType() const
{
	return ECharacterType::ECT_Player;
}

FCombatStats* AMainCharacter::GetCombatStats() const
{
	if (!CombatStats)
	{
		return nullptr;
	}

	CombatStats->Attack = AttributesComponent->GetAttack();
	CombatStats->Defence = AttributesComponent->GetDefense();
	CombatStats->Might = AttributesComponent->GetMight();
	CombatStats->Toughness = AttributesComponent->GetToughness();
	CombatStats->MinExtraDamage = AttributesComponent->GetModMinDamage();
	CombatStats->MaxExtraDamage = AttributesComponent->GetModMaxDamage();
	CombatStats->MinArmor = AttributesComponent->GetModMinArmor();
	CombatStats->MaxArmor = AttributesComponent->GetModMaxArmor();
	CombatStats->Reaction = AttributesComponent->GetReaction();
	CombatStats->Dexterity = AttributesComponent->GetDexterity();

	return CombatStats;
}
//CombatCharacter END

void AMainCharacter::ActivateLeftPunchCollistion()
{
	LeftPunchSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMainCharacter::DeactivateLeftPunchCollistion()
{
	LeftPunchSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainCharacter::ActivateRightPunchCollision()
{
	RightPunchSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMainCharacter::DeactivateRightPunchCollision()
{
	RightPunchSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainCharacter::PunchOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}

	CombatTarget = Cast<ACombatCharacter>(OtherActor);

	if (CombatTarget)
	{
		DealDamage(CombatTarget);		
	}
}

void AMainCharacter::DealDamage_Implementation(ACombatCharacter* OtherActor)
{
	GameplayUtils::TryHit(this, OtherActor, GetInstigatorController(), DamageType);
}

void AMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainCharacter, CurrentWeapon);
}
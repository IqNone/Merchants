// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Creature.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Creatures/IdleBehaviours/IdleManager.h"
#include "Creatures/IdleBehaviours/IdleBehaviourComponent.h"
#include "Creatures/IdleBehaviours/WalkBehaviourComponent.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Controller.h"
#include "Engine/DataTable.h"
#include "Engine/World.h"
#include "Interactables/Bag.h"

// Sets default values
ACreature::ACreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set up smooth rotaion
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	CreatureId = "None";

	IdleManager = CreateDefaultSubobject<UIdleManager>("IdleManager");
	WalkComponent = CreateDefaultSubobject<UWalkBehaviourComponent>("WalkComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ACreature::HandleTakeDamage);

	bDied = false;
}

// Called when the game starts or when spawned
void ACreature::BeginPlay()
{
	Super::BeginPlay();

	if (CreaturesDataTable)
	{
		CreatureData = CreaturesDataTable->FindRow<FCreatureData>(CreatureId, TEXT("Creature Data"));
	}

	if (CreatureData)
	{
		HealthComp->SetDefaultHealth(CreatureData->Health);
		SetMaxSpeed(CreatureData->WalkSpeed);
	}

	TInlineComponentArray<UIdleBehaviourComponent*> Components(this);
	GetComponents<UIdleBehaviourComponent>(Components);

	IdleManager->SetComponents(Components);
	IdleManager->Start();
}

// Called every frame
void ACreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACreature::SetMaxSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void ACreature::HandleTakeDamage(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("Health %f of %s"), Health, *GetName());

	IdleManager->End();

	if (Health <= 0 && !bDied)
	{
		bDied = true;

		GetMovementComponent()->StopMovementImmediately();		
		DetachFromControllerPendingDestroy();
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//IdleManager->End();		
		GetMesh()->GetAnimInstance()->Montage_Stop(0.f, nullptr);
		if (DeathAnim)
		{
			float Duration = GetMesh()->GetAnimInstance()->Montage_Play(DeathAnim, 1.f);
			GetWorld()->GetTimerManager().SetTimer(DeathMontageHandle, this, &ACreature::OnDeathMontageEnded, Duration, false);
		}
		else
		{
			OnDeathMontageEnded();
		}
	}
}

void ACreature::OnDeathMontageEnded() 
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	if (HasAuthority()) 
	{
		DropLoot();
		Destroy();
	}
}

void ACreature::DropLoot()
{
	if (!CreatureData)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABag* Bag = GetWorld()->SpawnActor<ABag>(BagClass, GetLootBagLocation(), FRotator::ZeroRotator, SpawnParams);
	Bag->InteractionDescription = GetCharacterName();
	Bag->SetLifeSpan(60.f);

	for (FDropItem DropItem : CreatureData->Drops)
	{		
		switch (DropItem.DropType)
		{
		case EDropItemType::EDIT_RandomQuantity:
			Bag->Items.Add(FItem(DropItem.ItemId, FMath::RandRange(DropItem.MinQuantity, DropItem.MaxQuantity)));
			break;
		case EDropItemType::EDIT_RandomChance:
			if (FMath::FRand() <= DropItem.Chanche)
			{
				Bag->Items.Add(FItem(DropItem.ItemId, 1));
			}
			break;
		default:
			break;
		}
	}
}

FVector ACreature::GetLootBagLocation()
{
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic);
	FCollisionQueryParams QueryParams;

	FVector Start = GetActorLocation();
	FVector End = FVector(Start.X, Start.Y, -1000000);

	GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectParams, QueryParams);
	
	if (Hit.bBlockingHit)
	{
		return Hit.Location;
	} 
	else
	{
		return GetActorLocation();
	}
}

//------------ Start CombatCharacter

float ACreature::GetMaxHealth() const
{
	return HealthComp->GetDefaultHealth();
}

float ACreature::GetHealth() const
{
	return HealthComp->GetHealth();
}

FText ACreature::GetCharacterName() const
{
	if (CreatureData)
	{
		return CreatureData->Name;
	}
	return FText();
}

float ACreature::GetRespawnSeconds() const
{
	if (CreatureData)
	{
		return CreatureData->RespawnSeconds;
	}
	return 0.0f;
}

//------------ End CombatCharacter
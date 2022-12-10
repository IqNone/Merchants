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

	Name = "Creature";

	// AI behaviour
	bAgressive = false;
	WalkSpeed = 150;
	RunSpeed = 600;

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

	SetMaxSpeed(WalkSpeed);

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

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMovementComponent()->StopMovementImmediately();
		DetachFromControllerPendingDestroy();

		if (HasAuthority())
		{
			SetLifeSpan(10.0f);
		}

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

FName ACreature::GetCharacterName() const
{
	return Name;
}

//------------ End CombatCharacter
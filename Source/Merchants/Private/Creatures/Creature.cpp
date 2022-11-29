// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Creature.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Creatures/IdleBehaviours/IdleManager.h"
#include "Creatures/IdleBehaviours/IdleBehaviourComponent.h"
#include "Creatures/IdleBehaviours/WalkBehaviourComponent.h"

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

	// AI behaviour
	bAgressive = false;
	WalkSpeed = 150;
	RunSpeed = 600;

	IdleManager = CreateDefaultSubobject<UIdleManager>("IdleManager");
	WalkComponent = CreateDefaultSubobject<UWalkBehaviourComponent>("WalkComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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
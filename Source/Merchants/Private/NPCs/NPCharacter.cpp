// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/NPCharacter.h"
#include "NPCs/NPCInteractable.h"
#include "NPCs/NPCDialogComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/TextRenderComponent.h"
#include "MainCharacter.h"

// Sets default values
ANPCharacter::ANPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	NameComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameComponent"));
	NameComponent->SetupAttachment(GetRootComponent());

	InteractableComponent = CreateDefaultSubobject<UChildActorComponent>("InteractableComponent");
	InteractableComponent->SetupAttachment(GetRootComponent());
	InteractableComponent->SetChildActorClass(ANPCInteractable::StaticClass());

	DialogComponent = CreateDefaultSubobject<UNPCDialogComponent>(TEXT("DialogComponent"));
}

// Called when the game starts or when spawned
void ANPCharacter::BeginPlay()
{
	Super::BeginPlay();
		
	if (HasAuthority())
	{
		if (ANPCInteractable* Interactable = Cast<ANPCInteractable>(InteractableComponent->GetChildActor()))
		{
			Interactable->SetOwner(this);
		}
	}
}

// Called every frame
void ANPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANPCharacter::StartDialog(AMainCharacter* Player)
{
	DialogComponent->Start();
	Player->OpenDialog(this);
}

void ANPCharacter::StopDialog(AMainCharacter* Player)
{
}
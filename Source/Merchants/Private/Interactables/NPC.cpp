// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/NPC.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "MainCharacter.h"

// Sets default values
ANPC::ANPC()
{
	InterationName = FText::FromString("Talk");

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());
	
	NameComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameComponent"));
	NameComponent->SetupAttachment(GetRootComponent());

	bReplicates = true;
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();

	InteractionDescription = FText::FromName(NPCName);
	NameComponent->SetText(FText::FromName(NPCName));
}

void ANPC::Interact(AMainCharacter* Player)
{
}


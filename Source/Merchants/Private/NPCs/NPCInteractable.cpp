// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/NPCInteractable.h"
#include "NPCs/NPCharacter.h"
#include "MainCharacter.h"
#include "Components/BoxComponent.h"

ANPCInteractable::ANPCInteractable()
{
	bReplicates = true;
	InterationName = FText::FromString("Talk");
}

void ANPCInteractable::BeginPlay()
{
	Super::BeginPlay();
	if (ANPCharacter* NPC = Cast<ANPCharacter>(GetOwner()))
	{
		InteractionDescription = FText::FromName(NPC->NPCName);
	}
}

void ANPCInteractable::Interact(AMainCharacter* Player)
{
	if (ANPCharacter * NPC = Cast<ANPCharacter>(GetOwner()))
	{
		NPC->StartDialog(Player);
	}
}

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
}

void ANPCInteractable::Interact(AMainCharacter* Player)
{
	if (ANPCharacter * MyOwner = Cast<ANPCharacter>(GetOwner()))
	{
		MyOwner->StartDialog(Player);
	}
}

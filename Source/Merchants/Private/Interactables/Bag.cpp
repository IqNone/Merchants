// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Bag.h"
#include "Components/StaticMeshComponent.h"
#include "MainCharacter.h"

ABag::ABag()
{
	InterationName = FText::FromString("Loot");

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
}

void ABag::Interact(AMainCharacter* Player)
{
	Player->OpenBag();
}

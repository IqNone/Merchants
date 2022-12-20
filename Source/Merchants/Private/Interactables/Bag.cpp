// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Bag.h"
#include "Components/StaticMeshComponent.h"
#include "MainCharacter.h"
#include "Net/UnrealNetwork.h"

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

void ABag::OnRep_Items(TArray<FItem> OldItems)
{
}

bool ABag::CanAdd(const FName ItemId, const int32 Quantity) const
{
	return false;
}

bool ABag::CanRemove(const FName ItemId, const int32 Quantity) const
{
	return false;
}

int32 ABag::Add(const FName ItemId, const int32 Quantity)
{
	return int32();
}

int32 ABag::Remove(const FName ItemId, const int32 Quantity)
{
	return int32();
}

void ABag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABag, Items);
}
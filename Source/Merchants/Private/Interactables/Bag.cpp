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
	OnItemsChanged.Broadcast();
}

bool ABag::CanAdd(const FName ItemId, const int32 Quantity) const
{
	return true;
}

bool ABag::CanRemove(const FName ItemId, const int32 Quantity) const
{
	return true;
}

int32 ABag::Add(const FName ItemId, const int32 Quantity)
{
	FItem* Existing = Items.FindByPredicate([ItemId](FItem Item)
		{
			return Item.ItemId == ItemId;
		});

	if (Existing)
	{
		Existing->Quantity += Quantity;
	}
	else
	{
		Items.Add(FItem(ItemId, Quantity));
	}

	return Quantity;
}

int32 ABag::Remove(const FName ItemId, const int32 Quantity)
{
	FItem* Existing = Items.FindByPredicate([ItemId](FItem Item)
		{
			return Item.ItemId == ItemId;
		});

	if (!Existing)
	{		
		return 0;
	}

	if (Existing->Quantity > Quantity)
	{
		Existing->Quantity -= Quantity;
		return Quantity;
	} 
	else
	{
		int32 result = Existing->Quantity;
		Items.RemoveAll([ItemId](FItem Item)
			{
				return Item.ItemId == ItemId;
			});
		return result;
	}
}

void ABag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABag, Items);
}
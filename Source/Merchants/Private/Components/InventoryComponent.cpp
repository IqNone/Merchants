// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	InventorySize = 72;

	Coins = 0;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

int32 UInventoryComponent::GetCoins() const
{
	return Coins;
}

void UInventoryComponent::AddCoins(int32 Ammount)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		Coins += Ammount;
	}
}

bool UInventoryComponent::RemoveCoins(int32 Ammount)
{
	if (GetOwnerRole() == ROLE_Authority && Ammount <= Coins)
	{
		Coins -= Ammount;
		return true;
	}
	return false;
}

bool UInventoryComponent::CanAdd(const FName ItemId, const int32 Quantity) const
{
	if (ItemId == CoinsItemId)
	{
		return true;
	}

	const FItem* Existing = Items.FindByPredicate([ItemId](FItem Item)
		{
			return Item.ItemId == ItemId;
		});

	return Existing || Items.Num() < InventorySize;
}

bool UInventoryComponent::CanRemove(const FName ItemId, const int32 Quantity) const
{
	if (ItemId == CoinsItemId) 
	{
		return Coins >= Quantity;
	}

	const FItem* Existing = Items.FindByPredicate([ItemId](FItem Item)
		{
			return Item.ItemId == ItemId;
		});

	return Existing && Existing->Quantity >= Quantity;	
}

int32 UInventoryComponent::Add(const FName ItemId, const int32 Quantity)
{
	if (ItemId == CoinsItemId)
	{
		AddCoins(Quantity);
		return Quantity;
	}

	if (GetOwnerRole() != ROLE_Authority)
	{
		return 0;
	}

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

int32 UInventoryComponent::Remove(const FName ItemId, const int32 Quantity)
{
	if (ItemId == CoinsItemId)
	{
		return RemoveCoins(Quantity) ? Quantity : 0;
	}

	if (GetOwnerRole() != ROLE_Authority)
	{
		return 0;
	}

	FItem* Existing = Items.FindByPredicate([ItemId](FItem Item)
		{
			return Item.ItemId == ItemId;
		});

	if (!Existing || Existing->Quantity < Quantity)
	{
		return 0;
	}

	Existing->Quantity -= Quantity;
	if (Existing->Quantity == 0)
	{
		Items.RemoveAll([ItemId](FItem Item)
			{
				return Item.ItemId == ItemId;
			});
	}

	return Quantity;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Coins);
	DOREPLIFETIME(UInventoryComponent, Items);
}

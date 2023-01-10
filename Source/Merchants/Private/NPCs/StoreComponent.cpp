// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/StoreComponent.h"

// Sets default values for this component's properties
UStoreComponent::UStoreComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UStoreComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (ItemsDataTable)
	{
		TArray<FStoreItem*> TempItems;
		ItemsDataTable->GetAllRows<FStoreItem>(TEXT("Store Component"), TempItems);

		for (auto Item : TempItems)
		{
			StoreItems.Add(*Item);
		}
	}
}


// Called every frame
void UStoreComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UStoreComponent::GetNPCSellPrice(FName ItemId) const
{
	const FStoreItem* Item = StoreItems.FindByPredicate([ItemId](FStoreItem Item)
		{
			return Item.ItemId == ItemId;
		});

	return !Item ? -1 : Item->StoreSellingPrice;
}

int UStoreComponent::GetNPCBuyPrice(FName ItemId) const
{
	const FStoreItem* Item = StoreItems.FindByPredicate([ItemId](FStoreItem Item)
		{
			return Item.ItemId == ItemId;
		});

	return !Item ? -1 : Item->StoreBuyingPrice;
}


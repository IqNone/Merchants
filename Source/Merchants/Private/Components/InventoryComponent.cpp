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

	Coins = 0;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UInventoryComponent::OnRep_Coins(int32 OldCoins)
{
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

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Coins);
}


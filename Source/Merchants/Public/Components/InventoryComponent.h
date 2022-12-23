// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Item.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MERCHANTS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InventoryComponent")
	int32 InventorySize;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "InventoryComponent")
	int32 Coins;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "InventoryComponent")
	TArray<FItem> Items;

	UPROPERTY(EditDefaultsOnly , Category = "InventoryComponent")
	FName CoinsItemId;

public:	

	int32 GetCoins() const;

	void AddCoins(int32 Ammount);

	bool RemoveCoins(int32 Ammount);
		
	bool CanAdd(const FName ItemId, const int32 Quantity) const;
	bool CanRemove(const FName ItemId, const int32 Quantity) const;
	int32 Add(const FName ItemId, const int32 Quantity);
	int32 Remove(const FName ItemId, const int32 Quantity);

	FORCEINLINE TArray<FItem> GetItems() { return Items; }
};

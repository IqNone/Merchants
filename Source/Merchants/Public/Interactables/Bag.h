// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Interactable.h"
#include "Items/Item.h"
#include "Bag.generated.h"

class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemsChanged);

UCLASS()
class MERCHANTS_API ABag : public AInteractable, public IItemsHolder
{
	GENERATED_BODY()
	
public:

	ABag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bag")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(ReplicatedUsing="OnRep_Items", VisibleAnywhere, BlueprintReadWrite, Category = "Bag|Items")
	TArray<FItem> Items;

	UPROPERTY(BlueprintAssignable, Category = "Bag|Items")
	FOnItemsChanged OnItemsChanged;

	virtual void Interact(AMainCharacter* Player) override;

public:

	UFUNCTION()
	void OnRep_Items(TArray<FItem> OldItems);

	// ItemsHolder
	virtual bool CanAdd(const FName ItemId, const int32 Quantity) const override;
	virtual bool CanRemove(const FName ItemId, const int32 Quantity) const override;
	virtual int32 Add(const FName ItemId, const int32 Quantity) override;
	virtual int32 Remove(const FName ItemId, const int32 Quantity) override;
};

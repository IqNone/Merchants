// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "StoreComponent.generated.h"

USTRUCT(BlueprintType)
struct FStoreItem : public FTableRowBase {

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StoreBuyingPrice = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StoreSellingPrice = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MERCHANTS_API UStoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStoreComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Store")
	UDataTable* ItemsDataTable;

	UPROPERTY(BlueprintReadOnly)
	TArray<FStoreItem> StoreItems;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// return -1 if the NPC doesn't sell this item
	int GetNPCSellPrice(FName ItemId) const;

	// return -1 if the NPC doesn't buy this item
	int GetNPCBuyPrice(FName ItemId) const;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Miscellaneous UMETA(DisplayName = "Miscellaneous"),
	EIT_Consumable UMETA(DisplayName = "Consumable"),
	EIT_Equipment UMETA(DisplayName = "Equipment"),
	EIT_Currency UMETA(DisplayName = "Currency"),

	EIT_MAX UMETA(DisplayName = "DefaultMax")
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	bool bStackable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	EItemType ItemType;
};

/**
 * 
 */
class MERCHANTS_API Item
{
public:
	Item(FName ItemId, int32 Quantity);

public:
	const FName ItemId;
	int32 Quantity;
};

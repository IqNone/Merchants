// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
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
	EItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	int32 Weight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	FText Description;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct MERCHANTS_API FItem
{
	GENERATED_BODY()

public:

	FItem();//Blueprint
	FItem(const FName ItemId, const int32 Quantity);
	FItem(const FItem& Other);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName ItemId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Quantity;
};

UINTERFACE(BlueprintType)
class MERCHANTS_API UItemsHolder : public UInterface
{
	GENERATED_BODY()
};

class MERCHANTS_API IItemsHolder
{
	GENERATED_BODY()

public:

	virtual bool CanAdd(const FName ItemId, const int32 Quantity) const = 0;
	virtual bool CanRemove(const FName ItemId, const int32 Quantity) const = 0;
	virtual int32 Add(const FName ItemId, const int32 Quantity) = 0;
	virtual int32 Remove(const FName ItemId, const int32 Quantity) = 0;
};
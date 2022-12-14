// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

FItem::FItem() : ItemId(""), Quantity(0)
{

}

FItem::FItem(const FName ItemId, const int32 Quantity): ItemId(ItemId), Quantity(Quantity)
{
}

FItem::FItem(const FItem& Other) : ItemId(Other.ItemId), Quantity(Other.Quantity)
{
}

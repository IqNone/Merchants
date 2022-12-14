// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Interactable.h"
#include "Items/Item.h"
#include "Bag.generated.h"

class UStaticMeshComponent;

UCLASS()
class MERCHANTS_API ABag : public AInteractable
{
	GENERATED_BODY()
	
public:

	ABag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bag")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bag")	
	TArray<FItem> Items;

	virtual void Interact(AMainCharacter* Player) override;
};

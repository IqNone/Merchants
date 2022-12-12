// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Interactable.h"
#include "Bag.generated.h"

UCLASS()
class MERCHANTS_API ABag : public AInteractable
{
	GENERATED_BODY()
	
public:

	ABag();

	int32 MaxItems;
	//TArray<

	virtual void Interact(AMainCharacter* Player) override;
};

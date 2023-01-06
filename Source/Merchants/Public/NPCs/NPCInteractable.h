// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Interactable.h"
#include "NPCInteractable.generated.h"

class ANPCharacter;
class AMainCharacter;

/**
 * 
 */
UCLASS()
class MERCHANTS_API ANPCInteractable : public AInteractable
{
	GENERATED_BODY()
	
public:
	ANPCInteractable();

	virtual void Interact(AMainCharacter* Player) override;

protected:

	virtual void BeginPlay() override;
};

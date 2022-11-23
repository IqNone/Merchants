// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Openable.generated.h"

class AMainCharacter;

/**
 * 
 */
UCLASS()
class MERCHANTS_API AOpenable : public AInteractable
{
	GENERATED_BODY()
	
public:

	AOpenable();

	UPROPERTY(EditAnywhere, Category = "Interaction", meta = (MakeEditWidget = "true"))
	FTransform NewLocationAndRotation;

	virtual void Interact(AMainCharacter* Player) override;

protected:
	
	virtual void BeginPlay() override;

private:

	FVector NewLocation;
	FRotator NewRotation;
};

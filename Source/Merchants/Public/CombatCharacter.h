// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatCharacter.generated.h"

UCLASS()
class MERCHANTS_API ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatCharacter();
	
	UFUNCTION(BlueprintPure)
	virtual float GetMaxHealth() const;

	UFUNCTION(BlueprintPure)
	virtual float GetHealth() const;

	UFUNCTION(BlueprintPure)
	virtual FText GetCharacterName() const;
};

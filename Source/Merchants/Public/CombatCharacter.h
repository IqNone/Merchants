// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	ECT_Player UMETA(DisplayName = "Player"),
	ECT_Creature UMETA(DisplayName = "Creature"),

	ECT_MAX UMETA(DisplayName = "DefaultMax")
};

struct FCombatStats
{
	int32 Attack;
	int32 Defence;
	int32 Might;
	int32 Toughness;
	int32 MinExtraDamage;
	int32 MaxExtraDamage;
	int32 MinArmor;
	int32 MaxArmor;
	int32 Reaction;
	int32 Dexterity;
};

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

	UFUNCTION(BlueprintPure)
	virtual ECharacterType GetCharacterType() const;

	virtual FCombatStats* GetCombatStats() const;
};

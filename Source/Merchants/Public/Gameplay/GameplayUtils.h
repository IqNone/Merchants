// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ACombatCharacter;
struct FCombatStats;

/**
 * 
 */
class MERCHANTS_API GameplayUtils
{
public:

	static void TryHit(ACombatCharacter* AttackerCharacter, ACombatCharacter* DefenderCharacter, AController* EventInstigator, TSubclassOf<UDamageType> DamageType);

private:

	static int32 CalculateDamage(FCombatStats* Attacker, FCombatStats* Defender);
};

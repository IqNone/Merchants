// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GameplayUtils.h"
#include "CombatCharacter.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

void GameplayUtils::TryHit(ACombatCharacter* AttackerCharacter, ACombatCharacter* DefenderCharacter, AController* EventInstigator, TSubclassOf<UDamageType> DamageType)
{
	if (!AttackerCharacter || !DefenderCharacter)
	{
		return;
	}

	FCombatStats* Attacker = AttackerCharacter->GetCombatStats();
	FCombatStats* Defender = DefenderCharacter->GetCombatStats();

	if (!Attacker || !Defender)
	{
		return;
	}

	// Let's see if the player makes the hit
	float AttackerDice = FMath::FRand() * Attacker->Dexterity;
	float DefenderDice = FMath::FRand() * Defender->Reaction;

	UE_LOG(LogTemp, Log, TEXT("Attacker (%f) vs Defender (%f)"), AttackerDice, DefenderDice);

	if (AttackerDice > DefenderDice)
	{
		int32 Damage = CalculateDamage(Attacker, Defender);
		UE_LOG(LogTemp, Log, TEXT("Damage Dealt: %d"), Damage);

		if (Damage > 0)
		{
			UGameplayStatics::ApplyDamage(DefenderCharacter, Damage, EventInstigator, AttackerCharacter, DamageType);
			// TODO MERC-35 Attacker receives exp
		}
		else
		{
			// TODO MERC-35 Defender receives exp
		}
	}
	else
	{
		// TODO MERC-35 Defender receives exp
	}
}

int32 GameplayUtils::CalculateDamage(FCombatStats* Attacker, FCombatStats* Defender)
{
	return Attacker->Attack / 10 + Attacker->Might / 2
		+ FMath::RandRange(Attacker->MinExtraDamage, Attacker->MaxExtraDamage)
		- Defender->Defence / 10 - Defender->Toughness / 2
		- FMath::RandRange(Defender->MinArmor, Defender->MaxArmor)

		// Make it fun!
		+ FMath::RandRange(-2, 9);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacter.h"

// Sets default values
ACombatCharacter::ACombatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

float ACombatCharacter::GetMaxHealth() const
{
	return 0.f;
}

float ACombatCharacter::GetHealth() const
{

	return 0.f;
}

FName ACombatCharacter::GetCharacterName() const
{
	return "Chad";
}

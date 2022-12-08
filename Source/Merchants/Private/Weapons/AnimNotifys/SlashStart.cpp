
// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AnimNotifys/SlashStart.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "MainCharacter.h"
#include "Weapons/Weapon.h"

void USlashStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}
	AMainCharacter* Main = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (!Main)
	{
		return;
	}
	AWeapon* Weapon = Main->GetCurrentWeapon();
	if (Weapon)
	{
		Weapon->ActivateCollision();
	}
}

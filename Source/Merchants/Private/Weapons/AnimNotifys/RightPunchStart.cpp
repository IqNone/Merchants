// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AnimNotifys/RightPunchStart.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "MainCharacter.h"

void URightPunchStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}
	AMainCharacter* Main = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (Main)
	{
		Main->ActivateRightPunchCollision();
	}
}

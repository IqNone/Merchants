// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PlayCombo.h"
#include "Components/Combat/CombatComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "MainCharacter.h"

void UPlayCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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
	UCombatComponent* CombatComponent = Main->GetCurrentCombatComponent();

	if (CombatComponent)
	{
		CombatComponent->ComboAttack();
	}
}

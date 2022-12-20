// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/CombatComponent.h"
#include "MainCharacter.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	// ...
	ComboType = EAnimComboType::EACT_Random;
	AnimIndex = -1;
	bAttacking = false;
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = GetOwner<AMainCharacter>();
	if (!Owner)
	{
		return;
	}

	AnimInstance = Owner->GetMesh()->GetAnimInstance();
}

void UCombatComponent::Attack()
{	
	if (bAttacking)
	{
		return; //already attacking
	}

	bAttacking = true;

	if (AnimIndex == -1)
	{
		PlayAnim();
	}
}

void UCombatComponent::StopAttack()
{	
	bAttacking = false;
}

void UCombatComponent::ComboAttack()
{
	if (bAttacking)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(TimerHandle);
		}

		PlayAnim();
	}
}

void UCombatComponent::Block()
{
}

void UCombatComponent::AnimEnd()
{
	AnimIndex = -1;
}

void UCombatComponent::PlayAnim()
{
	if (AnimInstance)
	{
		AnimIndex = GetNextAnimIndex();
		float Duration = AnimInstance->Montage_Play(Animations[AnimIndex]);
		Server_PlayAnim(AnimIndex);
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().SetTimer(TimerHandle, this, &UCombatComponent::AnimEnd, Duration, false);
		}
	}
	else
	{
		AnimEnd();
	}
}

void UCombatComponent::NetMulticast_PlayAnim_Implementation(int32 Index)
{
	if (!Owner->GetController())
	{
		AnimInstance->Montage_Play(Animations[Index]);
	}
}

void UCombatComponent::Server_PlayAnim_Implementation(int32 Index)
{
	NetMulticast_PlayAnim(Index);
}

int32 UCombatComponent::GetNextAnimIndex()
{
	switch (ComboType)
	{
	case EAnimComboType::EACT_Random:
		return FMath::RandRange(0, Animations.Num() - 1);
	case EAnimComboType::EACT_Consecutive:
	default:
		return (AnimIndex + 1) % Animations.Num();
	}
}

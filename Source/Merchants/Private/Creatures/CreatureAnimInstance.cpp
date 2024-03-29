// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/CreatureAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "KismetAnimationLibrary.h"

void UCreatureAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
}

void UCreatureAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		FVector Velocity = Pawn->GetVelocity();
		Velocity.Z = 0;

		Speed = Velocity.Size();
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Pawn->GetActorRotation());
	}
}
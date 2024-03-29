// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/IdleBehaviours/PlayAnimBehaviourComponent.h"
#include "Creatures/Creature.h"
#include "AIController.h"
#include "Engine/World.h"

UPlayAnimBehaviourComponent::UPlayAnimBehaviourComponent()
{	
	SectionName = "Default";
	MinAnimSeconds = 5;
	MaxAnimSeconds = 10;
	
	bHasEndAnim = true;	
	EndSectionName = "Default";

	SetIsReplicatedByDefault(true);
}

void UPlayAnimBehaviourComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!Owner)
	{
		return;
	}
	AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	World = GetWorld();
}

void UPlayAnimBehaviourComponent::StartBehaviour()
{
	if (!AnimInstance)
	{
		OnBehaviourEnd.Broadcast();
		return;
	}
	if (!Controller)
	{
		OnBehaviourEnd.Broadcast();
		return;
	}

	Controller->StopMovement();

	PlayMontage();
}

void UPlayAnimBehaviourComponent::EndImediatly()
{
	if (bIsPlayingEnd)
	{
		return; // Already ending
	} 
	else
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
		PlayEndMontage();
	}
}


void UPlayAnimBehaviourComponent::PlayMontage()
{
	bIsPlayingEnd = false;
	AnimInstance->Montage_Play(AnimMontage, 1.f);
	AnimInstance->Montage_JumpToSection(SectionName);

	NetMulticast_PlayMontage();

	float Seconds = FMath::RandRange(MinAnimSeconds, MaxAnimSeconds);

	World->GetTimerManager().SetTimer(TimerHandle, this, &UPlayAnimBehaviourComponent::OnMontageEnded, Seconds, false);
}

void UPlayAnimBehaviourComponent::OnMontageEnded()
{
	if (!AnimInstance)
	{
		OnBehaviourEnd.Broadcast();
		return;
	}

	AnimInstance->Montage_Stop(0, AnimMontage);
	NetMulticast_StopMontage();
	PlayEndMontage();
}


void UPlayAnimBehaviourComponent::PlayEndMontage()
{
	if (bHasEndAnim)
	{
		bIsPlayingEnd = true;
		float Duration = AnimInstance->Montage_Play(EndAnimMontage, 1.f);
		AnimInstance->Montage_JumpToSection(EndSectionName);
		NetMulticast_PlayEndMontage();
		World->GetTimerManager().SetTimer(TimerHandle, this, &UPlayAnimBehaviourComponent::OnEndMontageEnded, Duration, false);
	}
	else
	{
		OnBehaviourEnd.Broadcast();
	}
}

void UPlayAnimBehaviourComponent::OnEndMontageEnded()
{
	OnBehaviourEnd.Broadcast();
}

void UPlayAnimBehaviourComponent::NetMulticast_PlayMontage_Implementation()
{
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(AnimMontage, 1.f);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void UPlayAnimBehaviourComponent::NetMulticast_StopMontage_Implementation()
{
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0, AnimMontage);
	}
}

void UPlayAnimBehaviourComponent::NetMulticast_PlayEndMontage_Implementation()
{
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(EndAnimMontage, 1.f);
		AnimInstance->Montage_JumpToSection(EndSectionName);
	}
}
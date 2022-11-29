// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/IdleBehaviours/PauseBehaviourComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"


UPauseBehaviourComponent::UPauseBehaviourComponent()
{
	MaxPauseSeconds = 10;
	MinPauseSeconds = 5;
}

void UPauseBehaviourComponent::StartBehaviour()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		EndBehaviour();
		return;
	}

	float Seconds = FMath::RandRange(MinPauseSeconds, MaxPauseSeconds);

	World->GetTimerManager().SetTimer(TimerHandle, this, &UPauseBehaviourComponent::EndBehaviour, Seconds, false);
}

void UPauseBehaviourComponent::EndImediatly()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		EndBehaviour();
		return;
	}

	World->GetTimerManager().ClearTimer(TimerHandle);
	EndBehaviour();
}


void UPauseBehaviourComponent::EndBehaviour()
{
	OnBehaviourEnd.Broadcast();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/IdleBehaviours/IdleBehaviourComponent.h"
#include "Creatures/Creature.h"
#include "AIController.h"

// Sets default values for this component's properties
UIdleBehaviourComponent::UIdleBehaviourComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bCanPlayConsecutive = true;
	Weight = 1;
}

// Called when the game starts
void UIdleBehaviourComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner<ACreature>();

	if (Owner)
	{
		Controller = Owner->GetController<AAIController>();
	}
}

void UIdleBehaviourComponent::StartBehaviour()
{
	OnBehaviourEnd.Broadcast();
}

void UIdleBehaviourComponent::EndImediatly()
{
	OnBehaviourEnd.Broadcast();
}

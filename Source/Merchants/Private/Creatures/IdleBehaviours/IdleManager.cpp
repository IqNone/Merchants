// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/IdleBehaviours/IdleManager.h"
#include "Creatures/IdleBehaviours/WalkBehaviourComponent.h"
#include "Creatures/IdleBehaviours/IdleBehaviourComponent.h"

// Sets default values for this component's properties
UIdleManager::UIdleManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	Index = -1;
}

// Called when the game starts
void UIdleManager::BeginPlay()
{
	Super::BeginPlay();


	// ...
}

void UIdleManager::SetComponents(TInlineComponentArray<UIdleBehaviourComponent*> Components)
{
	IdleBehaviourComponents = Components;	

	for (UIdleBehaviourComponent* Component : IdleBehaviourComponents)
	{

		Component->OnBehaviourEnd.AddDynamic(this, &UIdleManager::OnIdleBehaviourEnd);
	}
}

void UIdleManager::Start()
{
	if (IdleBehaviourComponents.Num() == 0)
	{
		return;
	}

	int32 Next = FMath::RandRange(0, IdleBehaviourComponents.Num() - 1);
	if (Next == Index && Index >= 0 && !IdleBehaviourComponents[Index]->bCanPlayConsecutive)
	{
		Next = (Next + 1) % IdleBehaviourComponents.Num();
	}

	if (Next >= 0)
	{
		Index = Next;
		IdleBehaviourComponents[Index]->StartBehaviour();
	}
}

void UIdleManager::End()
{
	if (Index >= 0)
	{
		IdleBehaviourComponents[Index]->EndImediatly();
	}
}

void UIdleManager::OnIdleBehaviourEnd()
{
	Start();
}

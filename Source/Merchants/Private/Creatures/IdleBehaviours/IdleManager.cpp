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
	bEndCalled = false;
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

	WeightsSum = 0;
	Weights.Empty();

	for (UIdleBehaviourComponent* Component : IdleBehaviourComponents)
	{
		Component->OnBehaviourEnd.AddDynamic(this, &UIdleManager::OnIdleBehaviourEnd);
		WeightsSum += Component->Weight;
		Weights.Add(WeightsSum);
	}
}

void UIdleManager::Start()
{
	if (IdleBehaviourComponents.Num() == 0)
	{
		return;
	}

	bEndCalled = false;

	int32 Next = RandomIndex();
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
	bEndCalled = true;
	if (Index >= 0)
	{
		IdleBehaviourComponents[Index]->EndImediatly();
	}
}

void UIdleManager::OnIdleBehaviourEnd()
{
	if (!bEndCalled)
	{
		Start();
	}	
}

int32 UIdleManager::RandomIndex()
{
	int32 Rand = FMath::RandRange(0, WeightsSum - 1);

	// One could do a binary search here, but i don't think there will ever be more than 4-5 items
	for (int32 Id = 0; Id < Weights.Num(); ++Id)
	{
		if (Rand < Weights[Id])
		{
			return Id;
		}
	}

	return 0; //should not get here
}

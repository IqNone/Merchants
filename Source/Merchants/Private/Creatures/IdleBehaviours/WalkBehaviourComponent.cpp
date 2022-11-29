// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/IdleBehaviours/WalkBehaviourComponent.h"
#include "Creatures/Creature.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"

UWalkBehaviourComponent::UWalkBehaviourComponent()
{
	MinWalkDistance = 1000;
	MaxWalkDistance = 2500;

	PrimaryComponentTick.bCanEverTick = false;
}

void UWalkBehaviourComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Controller)
	{
		Controller->ReceiveMoveCompleted.AddDynamic(this, &UWalkBehaviourComponent::OnWalkCompleted);
	}
}

void UWalkBehaviourComponent::StartBehaviour()
{
	if (!Owner || !Controller)
	{
		OnWalkCompleted();
		return;
	}
	
	// Generate a random walk distance
	float Distance = UKismetMathLibrary::RandomFloatInRange(MinWalkDistance, MaxWalkDistance);

	FNavLocation Result;

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	bool Success = NavigationSystem->GetRandomReachablePointInRadius(Owner->GetActorLocation(), Distance, Result);

	if (Success)
	{
		EPathFollowingRequestResult::Type ResultType = Controller->MoveToLocation(Result.Location, 0.1f, true, true, true, true, nullptr, true);
		if (ResultType != EPathFollowingRequestResult::Type::RequestSuccessful)
		{
			UE_LOG(LogTemp, Warning, TEXT("MoveToTarget failed!"));
			OnWalkCompleted();
		}
	}
	else
	{
		OnWalkCompleted();
	}
}

void UWalkBehaviourComponent::EndImediatly()
{
	if (Controller)
	{
		Controller->StopMovement();
		OnWalkCompleted();
	}
}

void UWalkBehaviourComponent::OnWalkCompleted()
{
	OnBehaviourEnd.Broadcast();
}

void UWalkBehaviourComponent::OnWalkCompleted(FAIRequestID RequestID, EPathFollowingResult::Type MovementResult)
{
	OnWalkCompleted();
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IdleManager.generated.h"

class UWalkBehaviourComponent;
class UIdleBehaviourComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MERCHANTS_API UIdleManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIdleManager();

public:

	void Start();

	void End();

	void SetComponents(TInlineComponentArray<UIdleBehaviourComponent*> Components);

private:

	bool bEndCalled;

	TInlineComponentArray<UIdleBehaviourComponent*> IdleBehaviourComponents;
	int32 Index;

	TArray<int32> Weights;
	int32 WeightsSum;

	int32 RandomIndex();

	UFUNCTION()
	void OnIdleBehaviourEnd();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};

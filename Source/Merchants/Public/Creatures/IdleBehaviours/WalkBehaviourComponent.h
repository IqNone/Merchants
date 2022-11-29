// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/IdleBehaviours/IdleBehaviourComponent.h"
#include "WalkBehaviourComponent.generated.h"

struct FAIRequestID;
struct FPathFollowingResult;
namespace EPathFollowingResult {
	enum Type;
};

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MERCHANTS_API UWalkBehaviourComponent : public UIdleBehaviourComponent
{
	GENERATED_BODY()
	
public:

	UWalkBehaviourComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Walk")
	float MaxWalkDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Walk")
	float MinWalkDistance;

protected:
	virtual void BeginPlay() override;

public:

	virtual void StartBehaviour();

	virtual void EndImediatly();

private:

	void OnWalkCompleted();

	UFUNCTION()
	void OnWalkCompleted(FAIRequestID RequestID, EPathFollowingResult::Type MovementResult);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/IdleBehaviours/IdleBehaviourComponent.h"
#include "PauseBehaviourComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MERCHANTS_API UPauseBehaviourComponent : public UIdleBehaviourComponent
{
	GENERATED_BODY()
	
public:
	
	UPauseBehaviourComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Idle|Pause")
	float MinPauseSeconds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Idle|Pause")
	float MaxPauseSeconds;

public:

	virtual void StartBehaviour() override;

	virtual void EndImediatly() override;

private:

	FTimerHandle TimerHandle;

	UFUNCTION()
	void EndBehaviour();
};

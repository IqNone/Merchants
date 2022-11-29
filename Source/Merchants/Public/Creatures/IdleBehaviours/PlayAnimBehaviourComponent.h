// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/IdleBehaviours/IdleBehaviourComponent.h"
#include "PlayAnimBehaviourComponent.generated.h"

class UAnimMontage;
class UAnimInstance;
class UWorld;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MERCHANTS_API UPlayAnimBehaviourComponent : public UIdleBehaviourComponent
{
	GENERATED_BODY()
	
public:

	UPlayAnimBehaviourComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	FName SectionName;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	float MinAnimSeconds;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	float MaxAnimSeconds;

	UPROPERTY(EditDefaultsOnly, Category = "Animations|End")
	bool bHasEndAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations|End")
	UAnimMontage* EndAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations|End")
	FName EndSectionName;

protected:
	virtual void BeginPlay() override;

	UAnimInstance* AnimInstance;

public:

	virtual void StartBehaviour() override;

	virtual void EndImediatly() override;

private:

	UWorld* World;

	FTimerHandle TimerHandle;

	bool bIsPlayingEnd;

	void PlayMontage();
	void OnMontageEnded();

	void PlayEndMontage();
	void OnEndMontageEnded();
};


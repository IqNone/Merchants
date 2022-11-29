// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IdleBehaviourComponent.generated.h"

class ACreature;
class AAIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBehaviourEndSignature);

UCLASS( ClassGroup=(Custom))
class MERCHANTS_API UIdleBehaviourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIdleBehaviourComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Idle")
	bool bCanPlayConsecutive;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	ACreature* Owner;
	AAIController* Controller;

public:	
	
	virtual void StartBehaviour();

	virtual void EndImediatly();
		
	FOnBehaviourEndSignature OnBehaviourEnd;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Creature.generated.h"

class UIdleManager;
class UWalkBehaviourComponent;

UCLASS()
class MERCHANTS_API ACreature : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACreature();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	bool bAgressive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UIdleManager* IdleManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWalkBehaviourComponent* WalkComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void SetMaxSpeed(float Speed);
};

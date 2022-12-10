// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatCharacter.h"
#include "Creature.generated.h"

class UIdleManager;
class UWalkBehaviourComponent;
class UHealthComponent;
class AController;
class UDamageType;
class UInputComponent;

UCLASS()
class MERCHANTS_API ACreature : public ACombatCharacter
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UWalkBehaviourComponent* WalkComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Creature")
	FName Name;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Creature")
	bool bDied;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* DeathAnim;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTakeDamage(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	FTimerHandle DeathMontageHandle;

	void SetMaxSpeed(float Speed);

	UFUNCTION()
	void OnDeathMontageEnded();

public:
	// implement CombatCharacter
	virtual float GetMaxHealth() const override;
	virtual float GetHealth() const override;
	virtual FName GetCharacterName() const override;
};

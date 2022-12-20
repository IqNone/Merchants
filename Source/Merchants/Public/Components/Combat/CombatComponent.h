// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class UAnimInstance;
class AMainCharacter;

UENUM(BlueprintType)
enum class EAnimComboType : uint8
{
	EACT_Random UMETA(DisplayName = "Random"),
	EACT_Consecutive UMETA(DisplayName = "Consecutive"),

	EACT_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MERCHANTS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TArray<UAnimMontage*> Animations;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	EAnimComboType ComboType;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void Attack();
	virtual void StopAttack();

	virtual void ComboAttack();

	virtual void Block();	

protected:

	virtual void AnimEnd();

	virtual void PlayAnim();

	UFUNCTION(NetMulticast, Unreliable)
	virtual void NetMulticast_PlayAnim(int32 Index);

	UFUNCTION(Server, Unreliable)
	virtual void Server_PlayAnim(int32 Index);

private:

	int32 AnimIndex;

	bool bAttacking;
		
	AMainCharacter* Owner;

	UAnimInstance* AnimInstance;

	FTimerHandle TimerHandle;

	int32 GetNextAnimIndex();
};

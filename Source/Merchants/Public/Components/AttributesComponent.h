// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MERCHANTS_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributesComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes|Defaults")
	int32 MinValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes|Defaults")
	int32 MaxValue;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Basic Attributes
	UPROPERTY(ReplicatedUsing = "ReEvaluateBasicAttributes")
	int32 Physique;
	UPROPERTY(ReplicatedUsing = "ReEvaluateBasicAttributes")
	int32 Coordination;
	UPROPERTY(ReplicatedUsing = "ReEvaluateBasicAttributes")
	int32 Reasoning;
	UPROPERTY(ReplicatedUsing = "ReEvaluateBasicAttributes")
	int32 Will;
	UPROPERTY(ReplicatedUsing = "ReEvaluateBasicAttributes")
	int32 Instinct;
	UPROPERTY(ReplicatedUsing = "ReEvaluateBasicAttributes")
	int32 Vitality;

	// Player Levels
	UPROPERTY(Replicated)
	int32 Attack;

	UPROPERTY(Replicated)
	int32 Defense;

	// Cross Attributes
	int32 Might;
	int32 Matter;
	int32 Toughness;
	int32 Charm;
	int32 Reaction;
	int32 Perception;
	int32 Rationality;
	int32 Dexterity;
	int32 Ethereality;

	// Stats
	int32 MaxEmu;
	int32 MaxHealth;
	int32 MaxMana;
	int32 MaxActionPoints;

	// Extras
	int32 ExtraResearch;
	int32 ExtraHarvExp;
	int32 ExtraAlchExp;
	int32 ExtraMagExp;
	int32 ExtraCombatExp;

	// Stats Modifier
	UPROPERTY(Replicated)
	int32 ModMinDamage;
	UPROPERTY(Replicated)
	int32 ModMaxDamage;
	UPROPERTY(Replicated)
	int32 ModMinArmor;
	UPROPERTY(Replicated)
	int32 ModMaxArmor;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangePhysiqueBy(int Delta);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangeCoordinationBy(int Delta);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangeReasoningBy(int Delta);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangeWillBy(int Delta);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangeInstinctBy(int Delta);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangeVitalityBy(int Delta);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangeModMinDamageBy(int Delta);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangeModMaxDamageBy(int Delta);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangeModMinArmorBy(int Delta);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangeModMaxArmorBy(int Delta);

protected:

	UFUNCTION()
	void ReEvaluateBasicAttributes();

public:

	UFUNCTION(BlueprintGetter) int32 GetPhysique() { return Physique; }
	UFUNCTION(BlueprintGetter) int32 GetCoordination() { return Coordination; }
	UFUNCTION(BlueprintGetter) int32 GetReasoning() { return Reasoning; }
	UFUNCTION(BlueprintGetter) int32 GetWill() { return Will; }
	UFUNCTION(BlueprintGetter) int32 GetInstinct() { return Instinct; }
	UFUNCTION(BlueprintGetter) int32 GetVitality() { return Vitality; }

	UFUNCTION(BlueprintGetter) int32 GetMight() { return Might; }
	UFUNCTION(BlueprintGetter) int32 GetMatter() { return Matter; }
	UFUNCTION(BlueprintGetter) int32 GetToughness() { return Toughness; }
	UFUNCTION(BlueprintGetter) int32 GetCharm() { return Charm; }
	UFUNCTION(BlueprintGetter) int32 GetReaction() { return Reaction; }
	UFUNCTION(BlueprintGetter) int32 GetPerception() { return Perception; }
	UFUNCTION(BlueprintGetter) int32 GetRationality() { return Rationality; }
	UFUNCTION(BlueprintGetter) int32 GetDexterity() { return Dexterity; }
	UFUNCTION(BlueprintGetter) int32 GetEthereality() { return Ethereality; }

	UFUNCTION(BlueprintGetter) int32 GetMaxEmu() { return MaxEmu; }
	UFUNCTION(BlueprintGetter) int32 GetMaxHealth() { return MaxHealth; }
	UFUNCTION(BlueprintGetter) int32 GetMaxMana() { return MaxMana; }
	UFUNCTION(BlueprintGetter) int32 GetMaxActionPoints() { return MaxActionPoints; }

	UFUNCTION(BlueprintGetter) int32 GetExtraResearch() { return ExtraResearch; }
	UFUNCTION(BlueprintGetter) int32 GetExtraHarvExp() { return ExtraHarvExp; }
	UFUNCTION(BlueprintGetter) int32 GetExtraAlchExp() { return ExtraAlchExp; }
	UFUNCTION(BlueprintGetter) int32 GetExtraMagExp() { return ExtraMagExp; }
	UFUNCTION(BlueprintGetter) int32 GetExtraCombatExp() { return ExtraCombatExp; }	

	UFUNCTION(BlueprintGetter) int32 GetAttack() { return Attack; }
	UFUNCTION(BlueprintGetter) int32 GetDefense() { return Defense; }

	UFUNCTION(BlueprintGetter) int32 GetModMinDamage() { return ModMinDamage; }
	UFUNCTION(BlueprintGetter) int32 GetModMaxDamage() { return ModMaxDamage; }
	UFUNCTION(BlueprintGetter) int32 GetModMinArmor() { return ModMinArmor; }
	UFUNCTION(BlueprintGetter) int32 GetModMaxArmor() { return ModMaxArmor; }
};

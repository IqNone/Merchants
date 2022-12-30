// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributesComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	Physique = MinValue;
	Coordination = MinValue;
	Reasoning = MinValue;
	Will = MinValue;
	Instinct = MinValue;
	Vitality = MinValue;
}


// Called when the game starts
void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ReEvaluateAll();
}


// Called every frame
void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributesComponent::ChangePhysiqueBy_Implementation(int Delta)
{
	Physique += Delta;
	ReEvaluateAll();
}

void UAttributesComponent::ChangeCoordinationBy_Implementation(int Delta)
{
	Coordination += Delta;
	ReEvaluateAll();
}

void UAttributesComponent::ChangeReasoningBy_Implementation(int Delta)
{
	Reasoning += Delta;
	ReEvaluateAll();
}

void UAttributesComponent::ChangeWillBy_Implementation(int Delta)
{
	Will += Delta;
	ReEvaluateAll();
}

void UAttributesComponent::ChangeInstinctBy_Implementation(int Delta)
{
	Instinct += Delta;
	ReEvaluateAll();
}

void UAttributesComponent::ChangeVitalityBy_Implementation(int Delta)
{
	Vitality += Delta;
	ReEvaluateAll();
}

void UAttributesComponent::ReEvaluateAll()
{
	Might = (Physique + Coordination) / 2;
	Matter = (Physique + Will) / 2;
	Toughness = (Physique + Vitality) / 2;
	Charm = (Instinct + Vitality) / 2;
	Reaction = (Coordination + Instinct) / 2;
	Perception = (Instinct + Reasoning) / 2;
	Rationality = (Reasoning + Will) / 2;
	Dexterity = (Reasoning + Coordination) / 2;
	Ethereality = (Will + Vitality) / 2;

	MaxEmu = Might * 20;
	MaxHealth = Matter * 5;
	MaxMana = Ethereality * 8;
	MaxActionPoints = Rationality * 20;
	
	ExtraResearch = Rationality;
	ExtraHarvExp = Rationality;
	ExtraAlchExp = Rationality / 2;
	ExtraMagExp = Rationality / 2;
	ExtraCombatExp = Rationality / 3;
}

void UAttributesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAttributesComponent, Physique);
	DOREPLIFETIME(UAttributesComponent, Coordination);
	DOREPLIFETIME(UAttributesComponent, Reasoning);
	DOREPLIFETIME(UAttributesComponent, Will);
	DOREPLIFETIME(UAttributesComponent, Instinct);
	DOREPLIFETIME(UAttributesComponent, Vitality);
}
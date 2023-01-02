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
	MinValue = 4;
	MaxValue = 52;

	Physique = MinValue;
	Coordination = MinValue;
	Reasoning = MinValue;
	Will = MinValue;
	Instinct = MinValue;
	Vitality = MinValue;

	Attack = 7;
	Defense = 7;

	ModMinArmor = 0;
	ModMaxArmor = 0;
	ModMinDamage = 0;
	ModMaxDamage = 0;
}


// Called when the game starts
void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ReEvaluateBasicAttributes();
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
	ReEvaluateBasicAttributes();
}

void UAttributesComponent::ChangeCoordinationBy_Implementation(int Delta)
{
	Coordination += Delta;
	ReEvaluateBasicAttributes();
}

void UAttributesComponent::ChangeReasoningBy_Implementation(int Delta)
{
	Reasoning += Delta;
	ReEvaluateBasicAttributes();
}

void UAttributesComponent::ChangeWillBy_Implementation(int Delta)
{
	Will += Delta;
	ReEvaluateBasicAttributes();
}

void UAttributesComponent::ChangeInstinctBy_Implementation(int Delta)
{
	Instinct += Delta;
	ReEvaluateBasicAttributes();
}

void UAttributesComponent::ChangeVitalityBy_Implementation(int Delta)
{
	Vitality += Delta;
	ReEvaluateBasicAttributes();
}

void UAttributesComponent::ChangeModMinDamageBy_Implementation(int Delta)
{
	ModMinDamage += Delta;
}

void UAttributesComponent::ChangeModMaxDamageBy_Implementation(int Delta)
{
	ModMaxDamage += Delta;
}

void UAttributesComponent::ChangeModMinArmorBy_Implementation(int Delta)
{
	ModMinArmor += Delta;
}

void UAttributesComponent::ChangeModMaxArmorBy_Implementation(int Delta)
{
	ModMaxArmor += Delta;
}

void UAttributesComponent::ReEvaluateBasicAttributes()
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

	DOREPLIFETIME(UAttributesComponent, Attack);
	DOREPLIFETIME(UAttributesComponent, Defense);

	DOREPLIFETIME(UAttributesComponent, ModMinArmor);
	DOREPLIFETIME(UAttributesComponent, ModMaxArmor);
	DOREPLIFETIME(UAttributesComponent, ModMinDamage);
	DOREPLIFETIME(UAttributesComponent, ModMaxDamage);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/NPCDialogComponent.h"

// Sets default values for this component's properties
UNPCDialogComponent::UNPCDialogComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UNPCDialogComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	for (auto Node : Dialog)
	{
		NodeByName.Add(Node->NodeName, Node);
	}
}

// Called every frame
void UNPCDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNPCDialogComponent::Start()
{
	CurrentNode = NodeByName[StartNode];
}

UTextAndAnswers::UTextAndAnswers()
{
	WidgetClass = ConstructorHelpers::FClassFinder<UDialogWidget>(TEXT("/Game/NPCs/Dialog/HUD/Answers")).Class;
}

TSubclassOf<UDialogWidget> UTextAndAnswers::GetWidgetClass() const
{	
	return WidgetClass;
}

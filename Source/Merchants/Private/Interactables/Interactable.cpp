// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Interactable.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "MainCharacter.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InterationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InterationBox"));
	RootComponent = InterationBox;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();	
}

void AInteractable::Interact(AMainCharacter* Player)
{
}


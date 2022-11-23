// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Openable.h"
#include "MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AOpenable::AOpenable()
{
	InterationName = "Open";
}

void AOpenable::BeginPlay()
{
	Super::BeginPlay();

	NewLocation = UKismetMathLibrary::TransformLocation(GetTransform(), NewLocationAndRotation.GetLocation());
	NewRotation = UKismetMathLibrary::TransformRotation(GetTransform(), NewLocationAndRotation.GetRotation().Rotator());
}

void AOpenable::Interact(AMainCharacter* Player)
{
	Player->SetActorLocationAndRotation(NewLocation, NewRotation);
}

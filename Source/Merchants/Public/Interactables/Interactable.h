// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UBoxComponent;
class UBillboardComponent;
class AMainCharacter;

UCLASS()
class MERCHANTS_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	UBoxComponent* InterationBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	FText InterationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractionDescription;

private:
	UBillboardComponent* Billboard;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Interact(AMainCharacter* Player);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCharacter.generated.h"

class UTextRenderComponent;
class AMainCharacter;
class AInteractable;
class UChildActorComponent;
class UNPCDialogComponent;

UCLASS()
class MERCHANTS_API ANPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	FName NPCName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UTextRenderComponent* NameComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UChildActorComponent* InteractableComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UNPCDialogComponent* DialogComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartDialog(AMainCharacter* Player);

	void StopDialog(AMainCharacter* Player);

	// returns -1 if it doesn't sell the item
	int GetNPCSellPrice(FName ItemId) const;

	// returns -1 if it doesn't buy the item
	int GetNPCBuyPrice(FName ItemId) const;
};

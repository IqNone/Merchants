// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Interactable.h"
#include "NPC.generated.h"

class USkeletalMeshComponent;
class UTextRenderComponent;
class AMainCharacter;

UCLASS()
class MERCHANTS_API ANPC : public AInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	FName NPCName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UTextRenderComponent* NameComponent;

protected:	
	virtual void BeginPlay() override;

public:	

	virtual void Interact(AMainCharacter* Player) override;
};

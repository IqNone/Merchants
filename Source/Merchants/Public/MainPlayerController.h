// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UUserWidget;
class UWidget;

/**
 * 
 */
UCLASS()
class MERCHANTS_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> MainHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	FName InventoryWidgetName;

public:

	void ToogleInventory();

protected:
	virtual void BeginPlay() override;

private:
	UUserWidget* MainHUDWidget;
	UWidget* InventoryWidget;
};

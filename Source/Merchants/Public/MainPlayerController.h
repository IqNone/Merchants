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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	FName BagWidgetName;

public:

	void ToogleInventory();

	void OpenBag();
	void CloseBag();

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpenBag();

protected:
	virtual void BeginPlay() override;

private:
	UUserWidget* MainHUDWidget;
	UWidget* InventoryWidget;
	UWidget* BagWidget;

public:

	UFUNCTION(BlueprintPure)
	FORCEINLINE UUserWidget* GetMainHUDWidget() { return MainHUDWidget; }

};

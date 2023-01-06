// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UUserWidget;
class UWidget;
class ANPCharacter;

/**
 * 
 */
UCLASS()
class MERCHANTS_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMainPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> MainHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	FName InventoryWidgetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	FName MinimapWidgetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	FName BagWidgetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	FName DialogWidgetName;

public:

	void ToogleInventory();
	void ToogleMinimap();

	void OpenBag();
	void CloseBag();

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpenBag();

	void ShowMouse();
	void StopShowingMouse();

	void OpenDialog(ANPCharacter* NPC);

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpenDialog(ANPCharacter* NPC);

protected:
	virtual void BeginPlay() override;

private:
	UUserWidget* MainHUDWidget;
	UWidget* InventoryWidget;
	UWidget* MinimapWidget;
	UWidget* BagWidget;
	UWidget* DialogWidget;

private:
	int32 OpenedWindows;
	bool bForceShowingMouse;

	UFUNCTION(BlueprintCallable)
	void HandleWindowVisibility(bool bOpened);

	void SetMouseVisibility();

public:

	UFUNCTION(BlueprintPure)
	FORCEINLINE UUserWidget* GetMainHUDWidget() { return MainHUDWidget; }
};

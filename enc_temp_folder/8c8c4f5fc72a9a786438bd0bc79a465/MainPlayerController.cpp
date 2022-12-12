// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	//bEnableClickEvents = true;
	//bEnableMouseOverEvents = true;

	if (MainHUDClass)
	{
		MainHUDWidget = CreateWidget<UUserWidget>(this, MainHUDClass);
		if (MainHUDWidget)
		{
			MainHUDWidget->AddToViewport();
			MainHUDWidget->SetVisibility(ESlateVisibility::Visible);			
		}
	}

	if (!MainHUDWidget)
	{
		return;
	}

	InventoryWidget = MainHUDWidget->GetWidgetFromName(InventoryWidgetName);	
}

void AMainPlayerController::ToogleInventory()
{
	if (!InventoryWidget)
	{
		return;
	}

	if (InventoryWidget->IsVisible())
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
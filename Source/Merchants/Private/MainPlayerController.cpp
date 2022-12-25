// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;

	SetInputMode(InputMode);
	bShowMouseCursor = true;
	//bEnableClickEvents = true;
	//bEnableMouseOverEvents = true;	

	if (GetLocalRole() != ROLE_AutonomousProxy)
	{
		return;
	}

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
	BagWidget = MainHUDWidget->GetWidgetFromName(BagWidgetName);
}

void AMainPlayerController::ToogleInventory()
{
	if (GetLocalRole() != ROLE_AutonomousProxy || !InventoryWidget)
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

void AMainPlayerController::OpenBag()
{
	if (GetLocalRole() != ROLE_AutonomousProxy || BagWidget)
	{
		BagWidget->SetVisibility(ESlateVisibility::Visible);
		OnOpenBag();
	}
}

void AMainPlayerController::CloseBag()
{
	if (GetLocalRole() != ROLE_AutonomousProxy || BagWidget)
	{
		BagWidget->SetVisibility(ESlateVisibility::Hidden);		
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "NPCs/NPCharacter.h"

AMainPlayerController::AMainPlayerController()
{
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	OpenedWindows = 0;
	bForceShowingMouse = false;

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
	MinimapWidget = MainHUDWidget->GetWidgetFromName(MinimapWidgetName);
	BagWidget = MainHUDWidget->GetWidgetFromName(BagWidgetName);
	DialogWidget = MainHUDWidget->GetWidgetFromName(DialogWidgetName);

	SetMouseVisibility();
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
		HandleWindowVisibility(false);
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		HandleWindowVisibility(true);
	}
}

void AMainPlayerController::ToogleMinimap()
{
	if (GetLocalRole() != ROLE_AutonomousProxy || !MinimapWidget)
	{
		return;
	}

	if (MinimapWidget->IsVisible())
	{
		MinimapWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		MinimapWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::OpenBag()
{
	if (GetLocalRole() == ROLE_AutonomousProxy && BagWidget && !BagWidget->IsVisible())
	{
		BagWidget->SetVisibility(ESlateVisibility::Visible);
		HandleWindowVisibility(true);
		OnOpenBag();
	}
}

void AMainPlayerController::CloseBag()
{
	if (GetLocalRole() == ROLE_AutonomousProxy && BagWidget && BagWidget->IsVisible())
	{
		BagWidget->SetVisibility(ESlateVisibility::Hidden);
		HandleWindowVisibility(false);
	}
}

void AMainPlayerController::HandleWindowVisibility(bool bOpened)
{
	if (bOpened)
	{
		++OpenedWindows;
	}
	else
	{
		--OpenedWindows;
	}

	SetMouseVisibility();
}

void AMainPlayerController::ShowMouse()
{
	bForceShowingMouse = true;
	SetMouseVisibility();
}

void AMainPlayerController::StopShowingMouse()
{
	bForceShowingMouse = false;
	SetMouseVisibility();
}

void AMainPlayerController::OpenDialog(ANPCharacter* NPC)
{
	if (GetLocalRole() == ROLE_AutonomousProxy && DialogWidget && !DialogWidget->IsVisible())
	{
		DialogWidget->SetVisibility(ESlateVisibility::Visible);
		HandleWindowVisibility(true);
		OnOpenDialog(NPC);
	}
}

void AMainPlayerController::SetMouseVisibility()
{
	if (OpenedWindows == 0 && !bForceShowingMouse)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}
}
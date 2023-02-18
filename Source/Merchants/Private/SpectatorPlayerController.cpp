// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectatorPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ASpectatorPlayerController::ASpectatorPlayerController()
{

}

void ASpectatorPlayerController::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR //Dont't create the HUD on the server side when running from editor
	if (GetLocalRole() != ROLE_AutonomousProxy)
	{
		return;
	}
#endif

	FInputModeUIOnly InputMode;
	SetShowMouseCursor(true);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

	SetInputMode(InputMode);

	if (MainMenuHUDClass)
	{
		MainMenuHUDWidget = CreateWidget<UUserWidget>(this, MainMenuHUDClass);
		if (MainMenuHUDWidget)
		{
			MainMenuHUDWidget->AddToViewport();
			MainMenuHUDWidget->SetVisibility(ESlateVisibility::Visible);

			Show();
		}
	}
}

void ASpectatorPlayerController::Show_Implementation()
{
}

void ASpectatorPlayerController::Login(FString Username, FString Password)
{
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectatorPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Dao/Repository.h"
#include "Kismet/GameplayStatics.h"

ASpectatorPlayerController::ASpectatorPlayerController()
{
	Repository = CreateDefaultSubobject<URepository>(TEXT("Repository"));	
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

void ASpectatorPlayerController::LoginFailed_Implementation(const FString& ErrorMessage)
{
}

void ASpectatorPlayerController::Login(FString Username, FString Password)
{
	Repository->Authenticate(Username, Password, [&] (RepositoryResponse<FString> Response) {
		if (Response.bSuccess)
		{			
#if WITH_EDITOR
			EditorLogin(*Response.Payload.Get());
#else
			ClientTravel(ServerUrl + TEXT("?Token=") + *Response.Payload.Get(), ETravelType::TRAVEL_Absolute, true);
#endif			
		}
		else
		{
			LoginFailed(Response.ErrorMessage);
		}
	});
}

#if WITH_EDITOR
void ASpectatorPlayerController::EditorLogin_Implementation(const FString& Token)
{	
	UGameplayStatics::OpenLevel(this, "/Game/Maps/Continent_WP", true, TEXT("Token=") + Token);
}
#endif


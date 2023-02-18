// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpectatorPlayerController.generated.h"

class UUserWidget;
class UWidget;

UCLASS()
class MERCHANTS_API ASpectatorPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ASpectatorPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> MainMenuHUDClass;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	UUserWidget* MainMenuHUDWidget;

public:

	UFUNCTION(BlueprintCallable)
	void Login(FString Username, FString Password);
};

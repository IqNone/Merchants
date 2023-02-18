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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* MainMenuHUDWidget;

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Menu")
	void Show();

public:

	UFUNCTION(BlueprintCallable, Category = "Menu|Actions")
	void Login(FString Username, FString Password);
};

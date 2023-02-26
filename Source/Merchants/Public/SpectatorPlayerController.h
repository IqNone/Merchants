// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpectatorPlayerController.generated.h"

class UUserWidget;
class UWidget;
class URepository;

UCLASS(Config = Game)
class MERCHANTS_API ASpectatorPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ASpectatorPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> MainMenuHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* MainMenuHUDWidget;

	UPROPERTY(Config)
	FString ServerUrl;

private:
	UPROPERTY()
	URepository* Repository;

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Menu")
	void Show();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Menu|Login")
	void LoginFailed(const FString& ErrorMessage);

public:

	UFUNCTION(BlueprintCallable, Category = "Menu|Actions")
	void Login(FString Username, FString Password);

public:

#if WITH_EDITOR 

	UFUNCTION(Server, Unreliable)
	void EditorLogin(const FString& JWT);
#endif

};

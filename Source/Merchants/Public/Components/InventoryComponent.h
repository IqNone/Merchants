// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MERCHANTS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_Coins, BlueprintReadOnly, Category = "InventoryComponent")
	int32 Coins;

	UFUNCTION()
	void OnRep_Coins(int32 OldCoins);

public:	

	int32 GetCoins() const;

	void AddCoins(int32 Ammount);

	bool RemoveCoins(int32 Ammount);
		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatCharacter.h"
#include "Engine/DataTable.h"
#include "Creature.generated.h"

class UIdleManager;
class UWalkBehaviourComponent;
class UHealthComponent;
class AController;
class UDamageType;
class UInputComponent;
class ABag;

UENUM(BlueprintType)
enum class EDropItemType : uint8
{
	EDIT_RandomQuantity UMETA(DisplayName = "RandomQuantity"),
	EDIT_RandomChance UMETA(DisplayName = "RandomChance"),

	EDIT_MAX UMETA(DisplayName = "DefaultMax")
};

USTRUCT(BlueprintType)
struct FDropItem {

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drops")
	FName ItemId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drops")
	EDropItemType DropType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drops", Meta = (ClampMin = "0", EditCondition = "DropType == EDropItemType::EDIT_RandomQuantity"))
	int32 MinQuantity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drops", Meta = (ClampMin = "0", EditCondition = "DropType == EDropItemType::EDIT_RandomQuantity"))
	int32 MaxQuantity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drops", Meta = (ClampMin = "0", ClampMax = "1",EditCondition = "DropType == EDropItemType::EDIT_RandomChance"))
	float Chanche;
};

USTRUCT(BlueprintType)
struct FCreatureData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Spawn)
	float RespawnSeconds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	bool bAgressive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float RunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Drops)
	TArray<FDropItem> Drops;
};

UCLASS()
class MERCHANTS_API ACreature : public ACombatCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACreature();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FName CreatureId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UDataTable* CreaturesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UIdleManager* IdleManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UWalkBehaviourComponent* WalkComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Creature")
	bool bDied;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Creature")
	TSubclassOf<ABag> BagClass;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* DeathAnim;

private:
	FCreatureData* CreatureData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTakeDamage(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(Server, Reliable)
	void DropLoot();

	FVector GetLootBagLocation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	FTimerHandle DeathMontageHandle;

	void SetMaxSpeed(float Speed);

	UFUNCTION()
	void OnDeathMontageEnded();

public:
	// implement CombatCharacter
	virtual float GetMaxHealth() const override;
	virtual float GetHealth() const override;
	virtual FText GetCharacterName() const override;

	// spawn
	float GetRespawnSeconds() const;
};

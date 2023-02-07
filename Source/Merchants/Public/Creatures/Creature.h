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
	EDropItemType DropType = EDropItemType::EDIT_RandomQuantity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drops", Meta = (ClampMin = "0", EditCondition = "DropType == EDropItemType::EDIT_RandomQuantity"))
	int32 MinQuantity = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drops", Meta = (ClampMin = "0", EditCondition = "DropType == EDropItemType::EDIT_RandomQuantity"))
	int32 MaxQuantity = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drops", Meta = (ClampMin = "0", ClampMax = "1",EditCondition = "DropType == EDropItemType::EDIT_RandomChance"))
	float Chanche = .0F;
};

USTRUCT(BlueprintType)
struct FCreatureData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText Name;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Spawn)
	float RespawnSeconds = 10.;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	bool bAgressive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float Health = 100.;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	int32 Attack = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	int32 Defence = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	int32 Might = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	int32 Toughness = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	int32 Reaction = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	int32 Dexterity = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	int32 MinArmor = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	int32 MaxArmor = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float WalkSpeed = .0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stats)
	float RunSpeed = .0;

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

	FCombatStats* CombatStats;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called before destroying the object
	virtual void BeginDestroy() override;

	UFUNCTION()
	void HandleTakeDamage(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

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
	virtual ECharacterType GetCharacterType() const override;
	virtual FCombatStats* GetCombatStats() const override;

	// spawn
	float GetRespawnSeconds() const;
};

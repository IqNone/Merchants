// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "CreatureSpawner.generated.h"

class USphereComponent;
class ACreature;
class UEnvQuery;
class UBillboardComponent;
class USkeletalMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MERCHANTS_API ACreatureSpawner : public AActor
{
	GENERATED_BODY()

public:
	ACreatureSpawner();

	UPROPERTY(VisibleDefaultsOnly, Category = "Spawn")
	USphereComponent* SpawnArea;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawn")
	TSubclassOf<ACreature> CreatureClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawn")
	UBillboardComponent* Billboard;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawn")
	USkeletalMeshComponent* PreviewMesh;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float RespawnDeltaSeconds;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float MaxRoamingRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	UEnvQuery* SpawnQuery;

	UPROPERTY(VisibleDefaultsOnly, Category = "Spawn")
	FName SpawnQueryRadiusParamName;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FEnvQueryRequest SpawnQueryRequest;

	UFUNCTION()
	void Spawn();

	void SpawnQueryFinished(TSharedPtr<FEnvQueryResult> SpawnQueryResult);

	UFUNCTION()
	void HandleCreatureKilled(AActor* VictimActor, AActor* KilledActor, class AController* KillerController);

	FTimerHandle RespawnTimerHandle;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "CreatureSpawner.generated.h"

class ACreature;
class USphereComponent;
class UEnvQuery;
class UBillboardComponent;

UCLASS()
class MERCHANTS_API ACreatureSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreatureSpawner();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<ACreature> CreatureClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	USkeletalMeshComponent* PreviewMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Spawn")
	USphereComponent* SpawnArea;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UBillboardComponent* Billboard;

	FEnvQueryRequest SpawnQueryRequest;

	void Spawn();

	void SpawnQueryFinished(TSharedPtr<FEnvQueryResult> SpawnQueryResult);

};

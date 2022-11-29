// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/CreatureSpawner.h"
#include "Creatures/Creature.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Components/BillboardComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"

// Sets default values
ACreatureSpawner::ACreatureSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnArea = CreateDefaultSubobject<USphereComponent>(TEXT("SpehereComp"));
	SpawnArea->SetSphereRadius(1000.f);
	RootComponent = SpawnArea;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());
	
	PreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewMeshComp"));
	PreviewMesh->SetupAttachment(GetRootComponent());

	MaxRoamingRadius = 50000.f;
	RespawnDeltaSeconds = 10;

	SpawnQueryRadiusParamName = "SpawnRadius";
}

// Called when the game starts or when spawned
void ACreatureSpawner::BeginPlay()
{
	Super::BeginPlay();	

	if (SpawnQuery) 
	{
		SpawnQueryRequest = FEnvQueryRequest(SpawnQuery);

		Spawn();
	}	
}

// Called every frame
void ACreatureSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACreatureSpawner::Spawn()
{
	if (!HasAuthority())
	{
		return;
	}

	SpawnQueryRequest.SetFloatParam(SpawnQueryRadiusParamName, SpawnArea->GetScaledSphereRadius());
	SpawnQueryRequest.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &ACreatureSpawner::SpawnQueryFinished);
}

void ACreatureSpawner::SpawnQueryFinished(TSharedPtr<FEnvQueryResult> SpawnQueryResult)
{
	if (!SpawnQueryResult->IsSuccessful())
	{
		return;
	}
	FVector SpawnLocation = SpawnQueryResult->GetItemAsLocation(0);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACreature* SpawnedCreature = GetWorld()->SpawnActor<ACreature>(CreatureClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	//if (SpawnedCreature)
	//{
	//	SpawnedCreature->GetHealthComp()->OnActorKilled.AddDynamic(this, &AELSpawner::HandleCreatureKilled);
	//}
}
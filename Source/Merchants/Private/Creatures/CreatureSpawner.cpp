// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/CreatureSpawner.h"
#include "Creatures/Creature.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Components/BillboardComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Components/HealthComponent.h"

// Sets default values
ACreatureSpawner::ACreatureSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnArea = CreateDefaultSubobject<USphereComponent>(TEXT("SpehereComp"));
	SpawnArea->SetSphereRadius(1000.f);
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnArea->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SpawnArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	RootComponent = SpawnArea;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());
	
	PreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewMeshComp"));
	PreviewMesh->SetupAttachment(GetRootComponent());

	MaxRoamingRadius = 50000.f;	

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
	if (SpawnedCreature)
	{
		SpawnedCreature->HealthComp->OnActorKilled.AddDynamic(this, &ACreatureSpawner::HandleCreatureKilled);
	}
}

void ACreatureSpawner::HandleCreatureKilled(AActor* VictimActor, AActor* KillerActor, class AController* KillerController) 
{
	ACreature* Creature = Cast<ACreature>(VictimActor);
	if (Creature)
	{
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ACreatureSpawner::Spawn, Creature->GetRespawnSeconds());
	}
}
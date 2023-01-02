// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/DamageType.h"
#include "Gameplay/GameplayUtils.h"
#include "Components/BoxComponent.h"
#include "CombatCharacter.h"
#include "MainCharacter.h"
#include "Components/AttributesComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AWeapon::OnEquipped()
{
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetOwner()))
	{
		MainCharacter->AttributesComponent->ChangeModMinDamageBy(15);
		MainCharacter->AttributesComponent->ChangeModMaxDamageBy(30);
	}
}

void AWeapon::OnUnequipped()
{
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetOwner()))
	{
		MainCharacter->AttributesComponent->ChangeModMinDamageBy(-15);
		MainCharacter->AttributesComponent->ChangeModMaxDamageBy(-30);
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);
}

void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		DealDamage(OtherActor);

		ACombatCharacter* Target = Cast<ACombatCharacter>(OtherActor);
		if (!Target)
		{
			return;
		}

		AMainCharacter* Main = Cast<AMainCharacter>(GetOwner());
		if (Main)
		{
			Main->CombatTarget = Target;
		}
	}
}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AWeapon::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::DealDamage_Implementation(AActor* OtherActor)
{
	GameplayUtils::TryHit(Cast<ACombatCharacter>(GetOwner()), Cast<ACombatCharacter>(OtherActor), GetOwner()->GetInstigatorController(), DamageType);
}
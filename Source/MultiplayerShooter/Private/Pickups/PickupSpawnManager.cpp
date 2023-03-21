// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupSpawnManager.h"
#include "Pickups/Pickup.h"

APickupSpawnManager::APickupSpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void APickupSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	StartSpawnPickupTimer(nullptr);
}

void APickupSpawnManager::SpawnPickup()
{
	int32 PickupClassesNum = PickupClasses.Num();
	if (PickupClassesNum > 0)
	{
		int32 PickupSelectionIndex = FMath::RandRange(0, PickupClassesNum - 1);
		SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[PickupSelectionIndex], GetActorTransform());

		if (HasAuthority() && SpawnedPickup)
		{
			SpawnedPickup->OnDestroyed.AddDynamic(this, &APickupSpawnManager::StartSpawnPickupTimer);
		}
	}
}

void APickupSpawnManager::SpawnPickupTimerFinished()
{
	if (HasAuthority())
	{
		SpawnPickup();
	}
}

void APickupSpawnManager::StartSpawnPickupTimer(AActor* DestroyedActor)
{
	GetWorldTimerManager().SetTimer(
		SpawnPickupTimer,
		this,
		&APickupSpawnManager::SpawnPickupTimerFinished,
		SpawnPickupTime
	);
}

void APickupSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


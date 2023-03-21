// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawnManager.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API APickupSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupSpawnManager();

protected:
	virtual void BeginPlay() override;

	void SpawnPickup();
	void SpawnPickupTimerFinished();

	UFUNCTION()
	void StartSpawnPickupTimer(AActor* DestroyedActor);

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APickup>> PickupClasses;

	UPROPERTY()
	APickup* SpawnedPickup;

private:
	FTimerHandle SpawnPickupTimer;

	UPROPERTY(EditAnywhere)
	float SpawnPickupTime = 5;

public:	
	virtual void Tick(float DeltaTime) override;

};

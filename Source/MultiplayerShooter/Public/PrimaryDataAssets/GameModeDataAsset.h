// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameModeDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UGameModeDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath GameModePath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText WinConditionDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LevelStartingTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WarmupTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MatchTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CooldownTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CountdownTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxBotsCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxPlayersCount = 4;
};

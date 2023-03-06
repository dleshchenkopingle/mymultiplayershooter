// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MatchmakingManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExecuteServerTravel, bool, bWasSuccessful);

/**
 * 
 */
UCLASS(Blueprintable)
class MULTIPLAYERSHOOTER_API UMatchmakingManager : public UObject
{
	GENERATED_BODY()

public:
	void StartMatch();

protected:
	void ShuffleLevelsPathsArray();

	void TravelTo(FString Path);

public:
	FOnExecuteServerTravel OnExecuteServerTravel;

protected:
	int32 CurrentLevelNum = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString PathOfLobby = "/Game/Maps/Lobby";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSoftObjectPath> LevelsPathsArray;
};

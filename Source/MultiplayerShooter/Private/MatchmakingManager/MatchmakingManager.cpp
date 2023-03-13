// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchmakingManager/MatchmakingManager.h"


void UMatchmakingManager::StartMatch()
{
	if (ensureMsgf(LevelsPathsArray.Num(), TEXT("UMatchmakingManager::StartMatch - LevelsArray is empty")))
	{
		if (CurrentLevelNum == 0)
		{
			ShuffleLevelsPathsArray();
		}
		else if (LevelsPathsArray.Num() == CurrentLevelNum)
		{
			CurrentLevelNum = 0;
			FString PathToTravel = PathOfLobby + "?listen";
			TravelTo(PathToTravel);
			return;
		}

		FString LevelToLoadPath = LevelsPathsArray[CurrentLevelNum++].GetLongPackageName();
		FString PathToTravel = LevelToLoadPath + "?listen";
		TravelTo(LevelToLoadPath);
	}
}

void UMatchmakingManager::ShuffleLevelsPathsArray()
{
	for (int i = LevelsPathsArray.Num() - 1; i >= 0; --i)
	{
		int j = FMath::Rand() % (i + 1);
		if (i != j) LevelsPathsArray.Swap(i, j);
	}
}

void UMatchmakingManager::TravelTo(FString Path)
{
	UE_LOG(LogTemp, Warning, TEXT("UMatchmakingManager::TravelTo - Path: %s"), *Path);

	UWorld* World = GetWorld();
	if (World && (World->IsNetMode(ENetMode::NM_ListenServer) || World->IsNetMode(ENetMode::NM_DedicatedServer)))
	{
		bool bSuccess = World->ServerTravel(Path, true);
		if (OnExecuteServerTravel.IsBound())
		{
			OnExecuteServerTravel.Broadcast(bSuccess);
		}
	}
}
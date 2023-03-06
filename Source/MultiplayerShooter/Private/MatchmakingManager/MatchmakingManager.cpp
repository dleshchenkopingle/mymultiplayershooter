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
			TravelTo(PathOfLobby);
			return;
		}

		FString LevelToLoadPath = LevelsPathsArray[CurrentLevelNum++].GetLongPackageName();
		//FString PathToTravel = LevelToLoadPath + "?listen";
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
	UWorld* World = GetWorld();
	if (World && World->IsServer())
	{
		//TArray<FString> TravelOptions = { TEXT("bStartMatchWhenReady=1"), TEXT("bHasCharacterSelect=0"), FString::Printf(TEXT("GameVotingSelectionBehavior=%d"), (int32)GetGameVotingSelectionBehavior()), FString::Printf(TEXT("GamesToWin=%d"), GetGamesToWin()), FString::Printf(TEXT("PostGameLobbyLength=%f"), ActiveGameMode->GetPostGameLobbyLength()) };
		//FURL TravelURL(*Path);

		//for (const FString& Option : TravelOptions)
		//{
		//	TravelURL.AddOption(*Option);
		//}

		bool bSuccess = World->ServerTravel(Path, true);
		if (OnExecuteServerTravel.IsBound())
		{
			OnExecuteServerTravel.Broadcast(bSuccess);
		}
	}
}
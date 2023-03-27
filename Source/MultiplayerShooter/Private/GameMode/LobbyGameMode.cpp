// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"
#include "GameState/LobbyGameState.h"
#include "PlayerState/LobbyPlayerState.h"
#include "PlayerController/LobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/GameInstanceBase.h"
#include "MatchmakingManager/MatchmakingManager.h"


void ALobbyGameMode::TryToStartMatch()
{
	const int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers >= NumberOfPlayersToStartGame)
	{
		if (CheckAllPlayersReady())
		{
			if (UGameInstanceBase* GameInstance = Cast<UGameInstanceBase>(UGameplayStatics::GetGameInstance(GetWorld())))
			{
				GameInstance->StartMatch();
			}
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Yellow,
				FString::Printf(TEXT("NumberOfPlayers < %i"), NumberOfPlayersToStartGame)
			);
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

bool ALobbyGameMode::CheckAllPlayersReady()
{
	if (ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(UGameplayStatics::GetGameState(this)))
	{
		for (APlayerState* PlayerState : LobbyGameState->PlayerArray)
		{
			if (ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState))
			{
				if (!LobbyPlayerState->GetIsReady())
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

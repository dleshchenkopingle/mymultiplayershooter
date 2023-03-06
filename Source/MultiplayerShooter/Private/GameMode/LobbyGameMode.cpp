// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"
#include "GameState/LobbyGameState.h"
#include "PlayerState/LobbyPlayerState.h"
#include "PlayerController/LobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/GameInstanceBase.h"
#include "MatchmakingManager/MatchmakingManager.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ALobbyGameMode::Tick(float DeltaSeconds)
{
	UpdatePlayersHUD();
}

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

void ALobbyGameMode::BeginPlay()
{
	UGameInstanceBase* GameInstance = Cast<UGameInstanceBase>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (ensureMsgf(GameInstance, TEXT("ALobbyGameMode::BeginPlay - GameInstance is nullptr")))
	{
		if (UMatchmakingManager* MatchmakingManager = GameInstance->GetMatchmakingManager())
		{
			MatchmakingManager->OnExecuteServerTravel.AddDynamic(this, &ALobbyGameMode::OnServerTravelExecuted);
		}
	}
}

void ALobbyGameMode::UpdatePlayersHUD()
{
	auto PlayerArray = GameState.Get()->PlayerArray;

	for (int32 i = 0; i < PlayerArray.Num(); ++i)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerArray[i]->GetPlayerController());

		if (ensureMsgf(LobbyPlayerController, TEXT("ALobbyGameMode::RefreshPlayersHUD - PlayerController is nullptr")))
		{
			LobbyPlayerController->UpdateHUD();
		}
	}
}

void ALobbyGameMode::RemovePlayersHUD()
{
	auto PlayerArray = GameState.Get()->PlayerArray;

	for (int32 i = 0; i < PlayerArray.Num(); ++i)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerArray[i]->GetPlayerController());

		if (ensureMsgf(LobbyPlayerController, TEXT("ALobbyGameMode::RemovePlayersHUD - PlayerController is nullptr")))
		{
			LobbyPlayerController->RemoveHUD();
		}
	}
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

void ALobbyGameMode::OnServerTravelExecuted(bool bWasSuccesful)
{
	if (bWasSuccesful)
	{
		RemovePlayersHUD();
	}
}
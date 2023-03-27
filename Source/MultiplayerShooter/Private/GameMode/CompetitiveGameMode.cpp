// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CompetitiveGameMode.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MainCharacter.h"


void ACompetitiveGameMode::PlayerEliminated(AMainCharacter* EliminatedCharacter, AShooterPlayerController* VictimController, AShooterPlayerController* AttackerController)
{
	Super::PlayerEliminated(EliminatedCharacter, VictimController, AttackerController);

	if (!VictimController)
	{
		return;
	}

	EliminatedPlayers.AddUnique(VictimController);
	if (GameState)
	{
		auto PlayerArray = GameState->PlayerArray;
		if (PlayerArray.Num() - EliminatedPlayers.Num() <= 1)
		{
			SkipMatchTime();
			SetMatchState(MatchState::Cooldown);
			CheckPlayersMatchState();
		}
	}
}

void ACompetitiveGameMode::RequestRespawn(AMainCharacter* EliminatedCharacter, AController* EliminatedController)
{
	// Detach character from the controller and destroy.
	if (!EliminatedCharacter) return;
	EliminatedCharacter->Reset();
	EliminatedCharacter->Destroy();
}
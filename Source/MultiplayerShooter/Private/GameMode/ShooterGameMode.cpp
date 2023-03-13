// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShooterGameMode.h"
#include "Character/MainCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "GameState/ShooterGameState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/ShooterPlayerController.h"
#include "PlayerState/ShooterPlayerState.h"
#include "GameInstance/GameInstanceBase.h"
#include "MatchmakingManager/MatchmakingManager.h"


namespace MatchState
{
	const FName Cooldown = FName(TEXT("Cooldown"));
}

AShooterGameMode::AShooterGameMode()
{
	bDelayedStart = true;
}

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();

	UGameInstanceBase* GameInstance = Cast<UGameInstanceBase>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (ensureMsgf(GameInstance, TEXT("AShooterGameMode::BeginPlay - GameInstance is nullptr")))
	{
		if (UMatchmakingManager* MatchmakingManager = GameInstance->GetMatchmakingManager())
		{
			MatchmakingManager->OnExecuteServerTravel.AddDynamic(this, &AShooterGameMode::OnServerTravelExecuted);
		}
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(*It))
		{
			ShooterPlayerController->CheckMatchState();
		}
	}
}

void AShooterGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f) StartMatch();
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f) SetMatchState(MatchState::Cooldown);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = WarmupTime + MatchTime + CooldownTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f) RestartGame();
	}

	UpdatePlayersHUD();
}

void AShooterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(*It))
		{
			ShooterPlayerController->OnMatchStateSet(MatchState);
		}
	}
}

void AShooterGameMode::UpdatePlayersHUD()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(*It))
		{
			ShooterPlayerController->UpdateHUD();
		}
	}
}

void AShooterGameMode::RemovePlayersHUD()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(*It))
		{
			ShooterPlayerController->RemoveHUD();
		}
	}
}

void AShooterGameMode::OnServerTravelExecuted(bool bWasSuccesful)
{
	if (bWasSuccesful)
	{
		RemovePlayersHUD();
	}
}

void AShooterGameMode::PlayerEliminated(AMainCharacter* EliminatedCharacter, AShooterPlayerController* VictimController, AShooterPlayerController* AttackerController)
{
	if (!EliminatedCharacter || !AttackerController || !VictimController) return;

	AShooterPlayerState* AttackerPlayerState = AttackerController->GetPlayerState<AShooterPlayerState>();
	AShooterPlayerState* VictimPlayerState = VictimController->GetPlayerState<AShooterPlayerState>();
	if (!AttackerPlayerState || !VictimPlayerState) return;

	// Need to check if it's suicide.
	if (AttackerPlayerState != VictimPlayerState)
	{
		AttackerPlayerState->UpdateScore();
		VictimPlayerState->UpdateDefeats();
	}
	EliminatedCharacter->Eliminated();

	AShooterGameState* ShooterGameState = GetGameState<AShooterGameState>();
	if (!ShooterGameState) return;

	// Update GameState Info
	ShooterGameState->UpdateTopScorePlayerStates(AttackerPlayerState);
}

void AShooterGameMode::RequestRespawn(AMainCharacter* EliminatedCharacter, AController* EliminatedController)
{
	// Detach character from the controller and destroy.
	if (!EliminatedCharacter) return;
	EliminatedCharacter->Reset();
	EliminatedCharacter->Destroy();

	// Respawn a new character with a random reborn-spot for the controller.
	if (!EliminatedController) return;
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	const int32 PlayerStartIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);

	RestartPlayerAtPlayerStart(EliminatedController, PlayerStarts[PlayerStartIndex]);
}

void AShooterGameMode::RestartGame()
{
	if (UGameInstanceBase* GameInstance = Cast<UGameInstanceBase>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->StartMatch();
	}
}
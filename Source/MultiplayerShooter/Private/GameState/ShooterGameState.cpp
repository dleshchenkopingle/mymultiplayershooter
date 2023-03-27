// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/ShooterGameState.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/ShooterPlayerController.h"
#include "PlayerState/ShooterPlayerState.h"

void AShooterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterGameState, TopScorePlayerStates);
	DOREPLIFETIME(AShooterGameState, TopScore);
}

void AShooterGameState::UpdateTopScorePlayerStates(AShooterPlayerState* PlayerState)
{
	if (!PlayerState) return;
	
	if (TopScorePlayerStates.Num() == 0)
	{
		TopScorePlayerStates.AddUnique(PlayerState);
		TopScore = PlayerState->GetScore();
		HandleTopScore();
		HandleTopScorePlayerStates();
	}
	else if (TopScore == PlayerState->GetScore())
	{
		TopScorePlayerStates.AddUnique(PlayerState);
		HandleTopScore();
		HandleTopScorePlayerStates();
	}
	else if (TopScore < PlayerState->GetScore())
	{
		TopScorePlayerStates.Empty();
		TopScorePlayerStates.AddUnique(PlayerState);
		TopScore = PlayerState->GetScore();
		HandleTopScore();
		HandleTopScorePlayerStates();
	}
}

void AShooterGameState::ResetScores()
{
	TopScorePlayerStates.Empty();
	HandleTopScore();
	HandleTopScorePlayerStates();
}

void AShooterGameState::HandleTopScore()
{
	AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!ShooterPlayerController) return;
	
	// Updating the TopScore in the HUD
	ShooterPlayerController->UpdateTopScore();
}

void AShooterGameState::OnRep_TopScorePlayerStates()
{
	AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!ShooterPlayerController) return;

	ShooterPlayerController->HandleMatchState();
}

void AShooterGameState::HandleTopScorePlayerStates()
{
	AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!ShooterPlayerController) return;
	
	// Updating the TopScorePlayer in the HUD
	ShooterPlayerController->UpdateTopScorePlayer();
}

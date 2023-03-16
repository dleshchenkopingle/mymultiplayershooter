// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/ShooterPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/ShooterPlayerController.h"
#include "Character/MainCharacter.h"


void AShooterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterPlayerState, Defeats);
}

void AShooterPlayerState::UpdateScore()
{
	SetScore(GetScore() + ScoreAmount);

	ShooterPlayerController = ShooterPlayerController ? ShooterPlayerController : Cast<AShooterPlayerController>(GetOwningController());
	if (!ShooterPlayerController) return;
	
	ShooterPlayerController->UpdatePlayerScore(GetScore());
}

void AShooterPlayerState::UpdateDefeats()
{
	Defeats += 1;

	ShooterPlayerController = ShooterPlayerController ? ShooterPlayerController : Cast<AShooterPlayerController>(GetOwningController());
	if (!ShooterPlayerController) return;
	
	ShooterPlayerController->UpdatePlayerDefeats(Defeats);
}

void AShooterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character ? Character : Cast<AMainCharacter>(GetPawn());
	if (Character)
	{
		ShooterPlayerController = ShooterPlayerController ? ShooterPlayerController : Cast<AShooterPlayerController>(GetOwningController());
		if (ShooterPlayerController)
		{
			ShooterPlayerController->UpdatePlayerScore(Score);
		}
	}
}

void AShooterPlayerState::OnRep_Defeats()
{
	ShooterPlayerController = ShooterPlayerController ? ShooterPlayerController : Cast<AShooterPlayerController>(GetOwningController());
	if (!ShooterPlayerController) return;

	ShooterPlayerController->UpdatePlayerDefeats(Defeats);
}
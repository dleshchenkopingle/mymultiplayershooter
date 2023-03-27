// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/LobbyPlayerController.h"
#include "HUD/LobbyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/LobbyGameMode.h"
#include "PlayerState/LobbyPlayerState.h"


void ALobbyPlayerController::UpdateHUD()
{
	LobbyHUD = LobbyHUD ? LobbyHUD : Cast<ALobbyHUD>(GetHUD());
	if (LobbyHUD) LobbyHUD->Update();
}

void ALobbyPlayerController::TryToStartMatch_Implementation()
{
	if (ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		LobbyGameMode->TryToStartMatch();
	}
}

void ALobbyPlayerController::ToggleIsReady_Implementation()
{
	if (ALobbyPlayerState* LobbyPlayerState = GetPlayerState<ALobbyPlayerState>())
	{
		LobbyPlayerState->ToggleIsReady();
	}
}

void ALobbyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHUD();
}
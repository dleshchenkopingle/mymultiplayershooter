// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/LobbyPlayerController.h"
#include "HUD/LobbyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/LobbyGameMode.h"
#include "PlayerState/LobbyPlayerState.h"


void ALobbyPlayerController::RemoveHUD_Implementation()
{
	LobbyHUD = LobbyHUD ? LobbyHUD : Cast<ALobbyHUD>(GetHUD());
	if (LobbyHUD) LobbyHUD->Remove();
}

void ALobbyPlayerController::UpdateHUD_Implementation()
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
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/LobbyPlayerState.h"
#include "PlayerController/LobbyPlayerController.h"

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
}

void ALobbyPlayerState::ToggleIsReady()
{
	if (HasAuthority())
	{
		bIsReady = !bIsReady;

		LobbyPlayerController = LobbyPlayerController ? LobbyPlayerController : Cast<ALobbyPlayerController>(GetOwningController());
		if (!LobbyPlayerController) return;

		LobbyPlayerController->TryToStartMatch();
	}
}

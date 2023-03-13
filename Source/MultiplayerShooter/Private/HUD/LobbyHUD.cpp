// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LobbyHUD.h"
#include "HUD/LobbyWidget.h"
#include "PlayerController/LobbyPlayerController.h"
#include "GameState/LobbyGameState.h"
#include "PlayerState/LobbyPlayerState.h"
#include "Kismet/GameplayStatics.h"


void ALobbyHUD::AddLobbyWidget()
{
	if (LobbyWidgetClass)
	{
		LobbyWidget = CreateWidget<ULobbyWidget>(GetWorld(), LobbyWidgetClass, FName("Lobby Widget"));
		if (!ensureMsgf(LobbyWidget, TEXT("ALobbyHUD::AddLobbyWidget - LobbyWidget is nullptr"))) return;

		Update();
		LobbyWidget->AddToViewport();
	}
}

void ALobbyHUD::Remove()
{
	if (!LobbyWidget) return;

	LobbyWidget->RemoveFromParent();
}

void ALobbyHUD::Update()
{
	if (!LobbyWidget) return;

	if (AGameStateBase* GameState = UGameplayStatics::GetGameState(this))
	{
		LobbyWidget->ClearWidget();
		auto PlayerArray = GameState->PlayerArray;
		for (int32 i = 0; i < PlayerArray.Num(); ++i)
		{
			if (ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerArray[i]))
			{
				LobbyWidget->AppendPlayer(LobbyPlayerState);
			}

		}
	}
}

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	AddLobbyWidget();
}

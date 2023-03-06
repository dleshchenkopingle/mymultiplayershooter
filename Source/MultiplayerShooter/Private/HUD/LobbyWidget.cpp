// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LobbyWidget.h"
#include "Components/TextBlock.h"
#include "PlayerState/LobbyPlayerState.h"


void ULobbyWidget::ClearWidget()
{
	PlayerCounter = 0;

	if (DisplayText)
	{
		DisplayText->SetText(FText());
	}
}

void ULobbyWidget::AppendPlayer(ALobbyPlayerState* PlayerState)
{
	if (PlayerState && DisplayText)
	{
		FText CurrentText = DisplayText->GetText();
		FString NewTextString = CurrentText.ToString() +
			'\n' +
			FString::FromInt(++PlayerCounter) +
			". " +
			PlayerState->GetPlayerName() +
			" " +
			(PlayerState->GetIsReady() ? "(Ready)" : "(Not Ready)");

		DisplayText->SetText(FText::FromString(NewTextString));
	}
}

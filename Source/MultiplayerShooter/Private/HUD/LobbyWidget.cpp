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
		FString CurrentString = DisplayText->GetText().ToString();

		if (PlayerCounter > 0) CurrentString = CurrentString + '\n';
		FString NewTextString = CurrentString +
			FString::FromInt(++PlayerCounter) +
			". " +
			PlayerState->GetPlayerName() +
			" " +
			(PlayerState->GetIsReady() ? "(Ready)" : "(Not Ready)");

		DisplayText->SetText(FText::FromString(NewTextString));
	}
}

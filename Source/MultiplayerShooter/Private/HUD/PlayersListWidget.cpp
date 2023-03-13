// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayersListWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"


void UPlayersListWidget::ClearWidget()
{
	PlayerCounter = 0;

	if (PlayersListText)
	{
		PlayersListText->SetText(FText());
	}
}

void UPlayersListWidget::AppendPlayer(APlayerState* PlayerState)
{
	if (PlayerState && PlayersListText)
	{
		FString CurrentString = PlayersListText->GetText().ToString();

		if (PlayerCounter > 0) CurrentString = CurrentString + '\n';
		FString NewTextString = CurrentString +
			FString::FromInt(++PlayerCounter) +
			". " +
			PlayerState->GetPlayerName();

		PlayersListText->SetText(FText::FromString(NewTextString));
	}
}

void UPlayersListWidget::ToggleWidget()
{
	bIsVisible = !bIsVisible;

	bIsVisible ? AddToViewport() : RemoveFromParent();
}
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

		//UGameInstanceBase* GameInstance = Cast<UGameInstanceBase>(UGameplayStatics::GetGameInstance(GetWorld()));
		//if (ensureMsgf(GameInstance, TEXT("ALobbyHUD::AddLobbyWidget - GameInstance is nullptr")))
		//{
		//	if (UMatchmakingManager* MatchmakingManager = GameInstance->GetMatchmakingManager())
		//	{
		//		MatchmakingManager->OnExecuteServerTravel.AddDynamic(this, &ALobbyHUD::OnServerTravelExecuted);
		//	}
		//}
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

	LobbyWidget->ClearWidget();

	if (ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(UGameplayStatics::GetGameState(this)))
	{
		auto PlayerArray = LobbyGameState->PlayerArray;
		
		for (int32 i = 0; i < PlayerArray.Num(); ++i)
		{
			if (ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerArray[i]))
			{
				LobbyWidget->AppendPlayer(LobbyPlayerState);
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,
						5.f,
						FColor::Red,
						FString::Printf(TEXT("Wrong Player State"))
					);
				}
			}
		}
	}
}

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	AddLobbyWidget();
}

//void ALobbyHUD::OnServerTravelExecuted(bool bWasSuccesful)
//{
//	if (bWasSuccesful)
//	{
//		if (LobbyWidget)
//		{
//			LobbyWidget->RemoveFromParent();
//		}
//	}
//}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void TryToStartMatch();

	/** Called when a Controller with a PlayerState leaves the game or is destroyed */
	virtual void Logout(AController* Exiting) override;

protected:
	bool CheckAllPlayersReady();

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 NumberOfPlayersToStartGame = 2;
};

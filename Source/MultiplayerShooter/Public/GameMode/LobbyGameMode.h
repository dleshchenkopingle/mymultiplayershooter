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
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Tick(float DeltaSeconds) override;

	void TryToStartMatch();

	/** Called when a Controller with a PlayerState leaves the game or is destroyed */
	virtual void Logout(AController* Exiting) override;

protected:
	virtual void BeginPlay() override;

	void UpdatePlayersHUD();
	void RemovePlayersHUD();

	bool CheckAllPlayersReady();

	UFUNCTION()
	void OnServerTravelExecuted(bool bWasSuccesful);

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 NumberOfPlayersToStartGame = 2;
};

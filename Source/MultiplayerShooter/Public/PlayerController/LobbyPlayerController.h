// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Client, Reliable)
	void RemoveHUD();

	UFUNCTION(Client, Reliable)
	void UpdateHUD();

	UFUNCTION(Server, Reliable)
	void TryToStartMatch();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ToggleIsReady();

private:
	UPROPERTY()
	class ALobbyHUD* LobbyHUD;
};

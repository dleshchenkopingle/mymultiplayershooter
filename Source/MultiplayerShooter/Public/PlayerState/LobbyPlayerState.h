// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ToggleIsReady();

	UFUNCTION(BlueprintPure)
	bool GetIsReady() const { return bIsReady; };

protected:
	UPROPERTY(Replicated)
	bool bIsReady = false;

	UPROPERTY()
	class ALobbyPlayerController* LobbyPlayerController;
};

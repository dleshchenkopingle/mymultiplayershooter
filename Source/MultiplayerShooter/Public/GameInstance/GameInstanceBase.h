// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGameInstanceBase();

	void StartMatch();

	class UMatchmakingManager* GetMatchmakingManager() const { return MatchmakingManager; };

protected:
	virtual void Init() override;

protected:
	UPROPERTY(EditAnywhere, Category = Match)
	TSubclassOf<class UMatchmakingManager> MatchmakingManagerClass;

	UPROPERTY()
	class UMatchmakingManager* MatchmakingManager;
};

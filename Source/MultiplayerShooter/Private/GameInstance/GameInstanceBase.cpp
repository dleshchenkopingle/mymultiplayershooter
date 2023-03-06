// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstance/GameInstanceBase.h"
#include "MatchmakingManager/MatchmakingManager.h"


UGameInstanceBase::UGameInstanceBase()
{
}

void UGameInstanceBase::StartMatch()
{
	if (ensure(MatchmakingManager))
	{
		MatchmakingManager->StartMatch();
	}
}

void UGameInstanceBase::Init()
{
	Super::Init();

	if (ensure(MatchmakingManagerClass))
	{
		MatchmakingManager = NewObject<UMatchmakingManager>(this, MatchmakingManagerClass);
	}
}

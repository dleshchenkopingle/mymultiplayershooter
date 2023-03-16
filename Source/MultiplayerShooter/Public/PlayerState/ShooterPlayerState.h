// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdateScore();

	void UpdateDefeats();

	virtual void OnRep_Score() override;
	UFUNCTION()
	void OnRep_Defeats();

private:
	UPROPERTY(EditAnywhere)
	float ScoreAmount = 1.f;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats = 0;

	UPROPERTY()
	class AShooterPlayerController* ShooterPlayerController;
	UPROPERTY()
	class AMainCharacter* Character;
};

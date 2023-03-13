// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AShooterPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	
	virtual void ReceivedPlayer() override;
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdatePlayerHealth(float Health, float MaxHealth);
	void UpdatePlayerScore(float Value);
	void UpdatePlayerDefeats(int32 Value);
	/** Display the text when player is eliminated */
	void DisplayDefeatedMsg();
	void UpdateWeaponAmmo(int32 AmmoAmount);
	void UpdateCarriedAmmo(int32 AmmoAmount);
	void UpdateWeaponType(const FString& WeaponType);
	void UpdateGrenade(int32 GrenadeAmount);
	/** Update the warmup time before matching or update the cooldown time after the match has finished */
	void UpdateAnnouncement(int32 Countdown);
	/** Update the match time after matching */
	void UpdateMatchCountDown(int32 Countdown);
	/** Update the top score player */
	void UpdateTopScorePlayer();
	/** Update the top score */
	void UpdateTopScore();
	void RefreshHUD();

	UFUNCTION(Client, Reliable)
	void SetHUDTime();

	UFUNCTION(Client, Reliable)
	void HandleMatchState();

	/** Once the game mode's MatchState is changed, the player controller's MatchState callback is going to be executed. */
	UFUNCTION(Client, Reliable)
	void OnMatchStateSet(FName State);

	UFUNCTION(Client, Reliable)
	void UpdateHUD();

	UFUNCTION(Client, Reliable)
	void RemoveHUD();

	UFUNCTION(Client, Reliable)
	void TogglePlayersListWidget();

	UFUNCTION(Server, Reliable)
	void RequestUpdateTimes();

	UFUNCTION(Server, Reliable)
	void CheckMatchState();

private:
	UPROPERTY()
	class AShooterHUD* ShooterHUD;

	/**
	 *	Sync
	 */
	UFUNCTION(Server, Reliable)
	void RequestServerTimeFromClient(float ClientRequestTime);

	UFUNCTION(Client, Reliable)
	void ReportServerTimeToClient(float ClientRequestTime, float ServerReportTime);

	void CheckTimeSync(float DeltaTime);

	FORCEINLINE float GetServerTime() const { return HasAuthority() ? GetWorld()->GetTimeSeconds() : GetWorld()->GetTimeSeconds() + SyncDiffTime; }

	UPROPERTY(EditAnywhere, Category = Sync)
	float SyncFreq = 5.f;
	
	float SyncDiffTime = 0.f;
	float SyncRunningTime = 0.f;

	/** Level starting time, MatchState on GameMode is EnteringMap */
	UPROPERTY(Replicated)
	float LevelStartingTime = 0.f;

	/** Warmup time, MatchState on GameMode is WaitingToStart */
	UPROPERTY(Replicated)
	float WarmupTime = 0.f;
	
	/** Match time, MatchState on GameMode is InProgress */
	UPROPERTY(Replicated)
	float MatchTime = 0.f;

	/** Cooldown time when MatchState is InProgress and the match countdown has finished */
	UPROPERTY(Replicated)
	float CooldownTime = 0.f;

	/** Help to distinguish 2 time seconds in the unit of integer when ticking */
	int32 CountdownInt = 0;

	/** Match State, once the game mode's match state is changed, the player controller will respond */
	UPROPERTY(Replicated)
	FName MatchState;

	void JoinMidGame(float LevelStarting, float Warmup, float Match, float Cooldown, FName State);
};

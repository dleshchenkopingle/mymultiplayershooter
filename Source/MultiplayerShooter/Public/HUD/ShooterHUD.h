// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterTypes/HUDPackage.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API AShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
	FORCEINLINE void SetHUDPackage(FHUDPackage Package) { HUDPackage = Package; }
	FORCEINLINE void SetHUDSpread(float Spread) { HUDPackage.CrosshairsCurrentSpread = Spread; }
	
	class UCharacterOverlay* GetCharacterOverlay();
	/** Add CharacterOverlay Widget when MatchState is InProgress */
	void AddCharacterOverlay();

	FORCEINLINE class UAnnouncementWidget* GetAnnouncement() const { return Announcement; }
	/** Update Announcement Widget when MatchState is WaitingToStart, because this MatchState is too early, so the HUD is not
	 ** available since the MatchState is set, we need to call this function in BeginPlay() */
	void UpdateAnnouncement();

	void Refresh();
	void Update();
	void Remove();
	void TogglePlayersListWidget();
	
protected:
	virtual void BeginPlay() override;
	
private:
	/** TSubclass of the CharacterOverlay Widget */
	UPROPERTY(EditAnywhere, Category = Match)
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	/** CharacterOverlay Widget, showing the basic properties of the character */
	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	/** TSubclass of the Announcement Widget */
	UPROPERTY(EditDefaultsOnly, Category = Match)
	TSubclassOf<class UUserWidget> AnnouncementClass;

	/** Announcement Widget */
	UPROPERTY()
	class UAnnouncementWidget* Announcement;

	UPROPERTY(EditAnywhere, Category = Match)
	TSubclassOf<class UUserWidget> PlayersListWidgetClass;

	UPROPERTY()
	class UPlayersListWidget* PlayersListWidget;
	
	/**
	 *	Draw HUD cross hairs
	 */
	void DrawCrosshairs(UTexture2D* Texture, const FVector2D& Spread);

	void AddAnnouncement();
	void InitPlayersListWidget();
	
	FHUDPackage HUDPackage;
	FVector2D ViewportCenter;
};

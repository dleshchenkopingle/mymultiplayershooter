// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ShooterHUD.h"
#include "Components/TextBlock.h"
#include "HUD/CharacterOverlay.h"
#include "HUD/AnnouncementWidget.h"
#include "HUD/PlayersListWidget.h"
#include "Character/MainCharacter.h"
#include "ShooterComponents/CombatComponent.h"
#include "PlayerController/ShooterPlayerController.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameMode.h"
#include "Blueprint/WidgetLayoutLibrary.h"


// The DrawHUD function will be automatically called when we set the default HUD as BP_ShooterHUD in BP_GameMode settings.
void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();

	if (AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(GetOwningPlayerController()))
	{
		FName CurrentMatchState = ShooterPlayerController->GetMatchState();
		if (GEngine && GEngine->GameViewport && CurrentMatchState == MatchState::InProgress)
		{
			FVector2D ViewportSize;
			GEngine->GameViewport->GetViewportSize(ViewportSize);
			ViewportCenter = ViewportSize * .5f;
			DrawCrosshairs(HUDPackage.CrosshairsCenter, FVector2D(0.f, 0.f));
			DrawCrosshairs(HUDPackage.CrosshairsLeft, FVector2D(-HUDPackage.CrosshairsCurrentSpread, 0.f));
			DrawCrosshairs(HUDPackage.CrosshairsRight, FVector2D(HUDPackage.CrosshairsCurrentSpread, 0.f));
			DrawCrosshairs(HUDPackage.CrosshairsTop, FVector2D(0.f, -HUDPackage.CrosshairsCurrentSpread));
			DrawCrosshairs(HUDPackage.CrosshairsBottom, FVector2D(0.f, HUDPackage.CrosshairsCurrentSpread));
		}
	}
}

UCharacterOverlay* AShooterHUD::GetCharacterOverlay()
{
	if (!CharacterOverlay)
	{
		//AddCharacterOverlay();
	}

	return CharacterOverlay;
}

void AShooterHUD::BeginPlay()
{
	Super::BeginPlay();

	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());

	AddAnnouncement();
	InitPlayersListWidget();
}

void AShooterHUD::AddCharacterOverlay()
{
	if (CharacterOverlayClass && !CharacterOverlay)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(GetWorld(), CharacterOverlayClass, FName("Character Overlay"));
		if (!CharacterOverlay)
		{
			return;
		}
	}

	if (!CharacterOverlay->IsInViewport())
	{
		Refresh();
		CharacterOverlay->AddToViewport();
	}
}

void AShooterHUD::UpdateAnnouncement()
{
	AddAnnouncement();
}

void AShooterHUD::Refresh()
{
	if (CharacterOverlay && CharacterOverlay->DefeatedMsg)
	{
		CharacterOverlay->DefeatedMsg->SetVisibility(ESlateVisibility::Hidden);
		if (CharacterOverlay->IsAnimationPlaying(CharacterOverlay->DefeatedMsgAnim))
		{
			CharacterOverlay->StopAnimation(CharacterOverlay->DefeatedMsgAnim);
		}
	}
	// We need player controller and player character because the data is stored there.
	if (AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(GetOwningPlayerController()))
	{
		ShooterPlayerController->UpdatePlayerHealth(100.f, 100.f);
		const AMainCharacter* MainCharacter = Cast<AMainCharacter>(ShooterPlayerController->GetCharacter());
		if (MainCharacter && MainCharacter->GetCombat())
		{
			ShooterPlayerController->UpdateGrenade(MainCharacter->GetCombat()->GetGrenadeAmount());
			ShooterPlayerController->UpdateCarriedAmmo(MainCharacter->GetCombat()->GetCarriedAmmo());
			ShooterPlayerController->UpdateWeaponType(MainCharacter->GetCombat()->GetEquippedWeaponTypeString());

			if (AWeapon* EquippedWeapon = MainCharacter->GetCombat()->GetEquippedWeapon())
			{
				ShooterPlayerController->UpdateWeaponAmmo(EquippedWeapon->GetAmmo());
			}
			else
			{
				ShooterPlayerController->UpdateWeaponAmmo(0);
			}
		}
	}
}

void AShooterHUD::Update()
{
	if (AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(GetOwningPlayerController()))
	{
		ShooterPlayerController->SetHUDTime();
		ShooterPlayerController->UpdateTopScore();
		ShooterPlayerController->UpdateTopScorePlayer();
	}

	if (PlayersListWidget && PlayersListWidget->GetIsVisible())
	{
		if (AGameStateBase* GameState = UGameplayStatics::GetGameState(this))
		{
			PlayersListWidget->ClearWidget();
			auto PlayerArray = GameState->PlayerArray;
			for (int32 i = 0; i < PlayerArray.Num(); ++i)
			{
				PlayersListWidget->AppendPlayer(PlayerArray[i].Get());
			}
		}
	}
}

void AShooterHUD::Remove()
{
	if (CharacterOverlay)
	{
		CharacterOverlay->RemoveFromParent();
	}

	if (Announcement)
	{
		Announcement->RemoveFromParent();
	}

	if (PlayersListWidget)
	{
		PlayersListWidget->RemoveFromParent();
	}
}

void AShooterHUD::TogglePlayersListWidget()
{
	if (PlayersListWidget)
	{
		PlayersListWidget->ToggleWidget();
	}
}

void AShooterHUD::DrawCrosshairs(UTexture2D* Texture, const FVector2D& Spread)
{
	if (!IsValid(Texture))
	{
		return;
	}

	DrawTexture(
		Texture,
		ViewportCenter.X - Texture->GetSizeX() * .5f + Spread.X,
		ViewportCenter.Y - Texture->GetSizeY() * .5f + Spread.Y,
		Texture->GetSizeX(),
		Texture->GetSizeY(),
		0.f,
		0.f,
		1.f,
		1.f,
		HUDPackage.CrosshairsColor
	);
}

void AShooterHUD::AddAnnouncement()
{
	if (AnnouncementClass && !Announcement)
	{
		Announcement = CreateWidget<UAnnouncementWidget>(GetWorld(), AnnouncementClass, FName("Announcement"));
		if (!Announcement)
		{
			return;
		}
	}

	if (!Announcement->IsInViewport())
	{
		Announcement->AddToViewport();
	}
}

void AShooterHUD::InitPlayersListWidget()
{
	if (PlayersListWidgetClass)
	{
		PlayersListWidget = CreateWidget<UPlayersListWidget>(GetWorld(), PlayersListWidgetClass, FName("PlayersList"));
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void AddLobbyWidget();

	FORCEINLINE class ULobbyWidget* GetLobbyWidget() const { return LobbyWidget; }

	void Remove();
	void Update();

protected:
	virtual void BeginPlay() override;

private:
	/** TSubclass of the Lobby Widget */
	UPROPERTY(EditAnywhere, Category = Match)
	TSubclassOf<class UUserWidget> LobbyWidgetClass;

	/** Lobby Widget, showing the basic info in lobby */
	UPROPERTY()
	class ULobbyWidget* LobbyWidget;
};

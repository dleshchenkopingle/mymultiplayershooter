// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ClearWidget();
	void AppendPlayer(class ALobbyPlayerState* PlayerState);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DisplayText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ReadyButton;

private:
	int32 PlayerCounter = 0;
};

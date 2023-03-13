// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayersListWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UPlayersListWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ClearWidget();
	void AppendPlayer(class APlayerState* PlayerState);
	void ToggleWidget();

	bool GetIsVisible() const { return bIsVisible; }

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PlayersListText;

private:
	int32 PlayerCounter = 0;

	bool bIsVisible = false;
};

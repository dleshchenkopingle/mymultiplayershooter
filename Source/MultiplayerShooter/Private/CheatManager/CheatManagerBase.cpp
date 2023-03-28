// Fill out your copyright notice in the Description page of Project Settings.


#include "CheatManager/CheatManagerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MainCharacter.h"
#include "ShooterComponents/CombatComponent.h"


void UCheatManagerBase::SetIsImmuned(bool bNewIsImmuned)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(PlayerCharacter))
	{
		MainCharacter->SetIsImmuned(bNewIsImmuned);
	}
}

void UCheatManagerBase::SetMaxHealth()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(PlayerCharacter))
	{
		MainCharacter->SetMaxHealth();
	}
}

void UCheatManagerBase::SetCurrentAmmoAmount(int32 AmmoAmount)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(PlayerCharacter))
	{
		if (UCombatComponent* CombatComponent = MainCharacter->GetCombat())
		{
			CombatComponent->SetCurrentAmmoAmount(AmmoAmount);
		}
	}
}
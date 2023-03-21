// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupWeapon.h"
#include "Weapon/Weapon.h"
#include "ShooterComponents/CombatComponent.h"
#include "Character/MainCharacter.h"
#include "Components/SphereComponent.h"

APickupWeapon::APickupWeapon()
{
	if (PickupCollision)
	{
		PickupCollision->SetWorldScale3D(FVector(5.f, 5.f, 5.f));
	}
}

void APickupWeapon::BeginPlay()
{
	Super::BeginPlay();

	PickupCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);

	if (WeaponClass && HasAuthority())
	{
		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, GetActorTransform());
		if (Weapon)
		{
			Weapon->SetReplicates(true);
		}
	}
}

void APickupWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor))
	{
		if (UCombatComponent* Combat = MainCharacter->GetCombat())
		{
			Combat->OnWeaponEquip.AddDynamic(this, &APickupWeapon::OnWeaponEquip);
		}
	}
}

void APickupWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor))
	{
		ClearOverlappingWeapon(MainCharacter);
	}
}

void APickupWeapon::OnWeaponEquip(AWeapon* EquippedWeapon, AMainCharacter* MainCharacter)
{
	if (!MainCharacter || !EquippedWeapon || !Weapon)
	{
		return;
	}

	if (EquippedWeapon == Weapon)
	{
		SpawnBuffEffectAttached(MainCharacter);
		ClearOverlappingWeapon(MainCharacter);
		Destroy();
	}
}

void APickupWeapon::ClearOverlappingWeapon(AMainCharacter* MainCharacter)
{
	if (UCombatComponent* Combat = MainCharacter->GetCombat())
	{
		Combat->OnWeaponEquip.RemoveDynamic(this, &APickupWeapon::OnWeaponEquip);
	}
}
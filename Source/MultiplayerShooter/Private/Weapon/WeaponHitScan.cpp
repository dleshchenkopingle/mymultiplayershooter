// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponHitScan.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapon/WeaponType.h"
#include "Sound/SoundCue.h"

void AWeaponHitScan::Fire(const FVector& TraceHitTarget)
{
	Super::Fire(TraceHitTarget);

	if (HasAuthority())
	{
		FireHitScan(TraceHitTarget);
	}
}

void AWeaponHitScan::FireHitScan(const FVector& TraceHitTarget)
{
	// A map to store the pair of <HitResult.GetActor(), TotalHitNums> so that we can do the ApplyDamage() once for each actor rather than loop it.
	TMap<AActor*, float> DamageForEachActor;
	
	const FVector& Start = GetWeaponMesh()->GetSocketLocation(FName("MuzzleFlash"));
	const FVector& Dir = TraceHitTarget - Start;
	
	// Scatter multiple lines when firing. For-loop each line.
	for (uint32 i = 0; i < ScatterNum; i++)
	{
		// Randomize the shooting direction for the scatter effect. ScatterDir is a normalized vector.
		const FVector& ScatterDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Dir, ScatterAngle);
		const FVector& End = Start + ScatterDir * TRACE_LENGTH;
		HitScan(DamageForEachActor, Start, End);
	}
	
	// Loop the map and apply the total damage for each actor.
	for (const auto& Pair : DamageForEachActor)
	{
		if (const APawn* InstigatorPawn = Cast<APawn>(GetOwner()))
		{
			UGameplayStatics::ApplyDamage(
				Pair.Key,
				Pair.Value,
				InstigatorPawn->GetController(),
				this,
				UDamageType::StaticClass());
		}
	}

	PlayMuzzleFlashEffect();
	PlayFireSoundEffect();
}

void AWeaponHitScan::HitScan(TMap<AActor*, float>& DamageForEachActor, const FVector& Start, const FVector& End)
{
	FHitResult HitResult;
	if (GetWorld())
	{
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);
	}
	if (!HitResult.bBlockingHit) return;
		
	PlayHitParticleEffect(HitResult.ImpactPoint);
	PlayHitSoundEffect();
	PlayBeamParticleEffect(Start, HitResult.ImpactPoint);

	// Insert the 'damage for each actor' pair into the map.
	if (DamageForEachActor.Contains(HitResult.GetActor()))
	{
		DamageForEachActor[HitResult.GetActor()] += Damage;
	}
	else
	{
		DamageForEachActor.Emplace(HitResult.GetActor(), Damage);
	}
}

void AWeaponHitScan::PlayMuzzleFlashEffect_Implementation()
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}
	}
}

void AWeaponHitScan::PlayFireSoundEffect_Implementation()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void AWeaponHitScan::PlayHitParticleEffect_Implementation(FVector_NetQuantize ImplactPoint)
{
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, ImplactPoint);
	}
}

void AWeaponHitScan::PlayHitSoundEffect_Implementation()
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
}

void AWeaponHitScan::PlayBeamParticleEffect_Implementation(FVector_NetQuantize Start, FVector_NetQuantize ImpactPoint)
{
	if (UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
		this,
		BeamParticle,
		Start
	))
	{
		Beam->SetVectorParameter(FName("Target"), ImpactPoint);
	}
}
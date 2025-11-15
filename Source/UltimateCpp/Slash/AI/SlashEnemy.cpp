// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashEnemy.h"

#include "DrawDebugHelpers.h"
#include "kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Interfaces/BC_WeaponInterface.h"
#include "Combat/BC_Weapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogSlashEnemy, All, All);

ASlashEnemy::ASlashEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASlashEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ASlashEnemy::TakeDamage_Implementation(AActor* Causer, float Damage, const FHitResult& Hit)
{
	Super::TakeDamage_Implementation(Causer, Damage, Hit);
	
	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, Hit.ImpactPoint);
	
	if (HitParticles)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitParticles, Hit.ImpactPoint);

	if (ABC_Weapon* Weapon = EquippedWeapon.Get())
	{
		IBC_WeaponInterface::Execute_EndAttackTracing(Weapon);
	}
}

// void ASlashEnemy::HandleDeath()
// {
// 	Super::HandleDeath();
// 	const FVector Direction = ((GetActorLocation() - UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation())*FVector(1.0f, 1.0f, 0.0f)).GetSafeNormal();
// 	GetMesh()->AddImpulse(Direction * DeathImpulse, TEXT("Spine2"));
// 	SetLifeSpan(LifeSpan);
// }





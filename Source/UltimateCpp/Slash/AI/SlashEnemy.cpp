// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashEnemy.h"

#include "DrawDebugHelpers.h"
#include "kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BC_MontageComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSlashEnemy, All, All);

ASlashEnemy::ASlashEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASlashEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ASlashEnemy::TakeDamage_Implementation(float Damage, const FHitResult& Hit)
{
	Super::TakeDamage_Implementation(Damage, Hit);
	
	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, Hit.ImpactPoint);
	
	if (HitParticles)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitParticles, Hit.ImpactPoint);
}

// void ASlashEnemy::HandleDeath()
// {
// 	Super::HandleDeath();
// 	const FVector Direction = ((GetActorLocation() - UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation())*FVector(1.0f, 1.0f, 0.0f)).GetSafeNormal();
// 	GetMesh()->AddImpulse(Direction * DeathImpulse, TEXT("Spine2"));
// 	SetLifeSpan(LifeSpan);
// }





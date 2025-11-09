// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashWeapon.h"

#include "NiagaraComponent.h"


ASlashWeapon::ASlashWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponTrailSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	WeaponTrailSystem->SetupAttachment(Mesh, TEXT("trail_socket"));
	WeaponTrailSystem->SetAutoActivate(false);
	WeaponTrailSystem->SetActive(false);
}

void ASlashWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASlashWeapon::SetWeaponTrailActive_Implementation(bool bWeaponTrailActive)
{
	if (WeaponTrailSystem)
		WeaponTrailSystem->SetActive(bWeaponTrailActive);
}

void ASlashWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


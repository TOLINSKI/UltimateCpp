// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashWeapon.h"

#include "Components/CapsuleComponent.h"


ASlashWeapon::ASlashWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASlashWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASlashWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Melee/BC_CapsuleWeapon.h"
#include "SlashWeapon.generated.h"

UCLASS()
class ULTIMATECPP_API ASlashWeapon : public ABC_CapsuleWeapon
{
	GENERATED_BODY()
public:
	ASlashWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

// Base Craft Interface
// ====================
public:
};

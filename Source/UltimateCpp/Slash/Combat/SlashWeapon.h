// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Melee/BC_CapsuleWeapon.h"
#include "Utility/SlashTypes.h"
#include "SlashWeapon.generated.h"

class UNiagaraComponent;

UCLASS()
class ULTIMATECPP_API ASlashWeapon : public ABC_CapsuleWeapon
{
	GENERATED_BODY()
public:
	ASlashWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Weapon")
	ESlashWeaponType WeaponType;

// Slash Interface
// ====================
public:
	virtual void SetWeaponTrailActive_Implementation(bool bWeaponTrailActive) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slash|Weapon|FX")
	TObjectPtr<UNiagaraComponent> WeaponTrailSystem;

public:
	ESlashWeaponType GetWeaponType() const { return WeaponType; }
};

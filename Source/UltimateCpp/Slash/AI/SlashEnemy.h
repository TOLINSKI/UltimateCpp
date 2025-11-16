// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/BC_Enemy.h"
#include "SlashEnemy.generated.h"

class UAnimMontage;
class USoundBase;
class USoundAttenuation;
class UNiagaraSystem;
class UBC_AttributeComponent;

UCLASS()
class ULTIMATECPP_API ASlashEnemy : public ABC_Enemy
{
	GENERATED_BODY()

public:
	ASlashEnemy();

// Slash Interface:
public:
	//~ Begin AI Interface
	virtual void QuickAttack() override;
	//~ End AI Interface
	
	//~ Begin Damageable Interface
	virtual void TakeDamage_Implementation(AActor* Causer, float Damage, const FHitResult& Hit) override;
	//~ End Damageable Interface
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|SFX")
	TObjectPtr<USoundBase> HitSound; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|VFX")
	TObjectPtr<UNiagaraSystem> HitParticles;
};

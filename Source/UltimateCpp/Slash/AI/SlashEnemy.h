// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/BC_AttackerInterface.h"
#include "Interfaces/BC_DamageableInterface.h"
#include "SlashEnemy.generated.h"

class UAnimMontage;
class USoundBase;
class USoundAttenuation;
class UNiagaraSystem;

UCLASS()
class ULTIMATECPP_API ASlashEnemy : public ACharacter, public IBC_AttackerInterface, public IBC_DamageableInterface
{
	GENERATED_BODY()

public:
	ASlashEnemy();

// Slash Interface:
public:
	virtual void TakeDamage_Implementation(const FVector& ImpactPoint) override;

private:
	FName GetHitReactMontageSectionName(const FVector& ImpactPoint) const;
	void PlayHitReactMontage(const FVector& ImpactPoint) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Animation")
	TObjectPtr<UAnimMontage> HitReactMontage; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|SFX")
	TObjectPtr<USoundBase> HitSound; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|VFX")
	TObjectPtr<UNiagaraSystem> HitParticles; 
};

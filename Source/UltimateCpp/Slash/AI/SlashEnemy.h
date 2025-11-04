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

UCLASS()
class ULTIMATECPP_API ASlashEnemy : public ACharacter, public IBC_AttackerInterface, public IBC_DamageableInterface
{
	GENERATED_BODY()

public:
	ASlashEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

// Slash Interface:
public:
	virtual void TakeDamage_Implementation(const FVector& ImpactPoint) override;

private:
	FName GetHitReactMontageSectionName(const FVector& ImpactPoint) const;
	void PlayHitReactMontage(const FVector& ImpactPoint) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Animation")
	TObjectPtr<UAnimMontage> HitReactMontage; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Animation")
	TObjectPtr<USoundBase> HitSound; 

public:
#if WITH_EDITORONLY_DATA
	UFUNCTION(CallInEditor, BlueprintCallable)
	void PlaySound();
#endif
};

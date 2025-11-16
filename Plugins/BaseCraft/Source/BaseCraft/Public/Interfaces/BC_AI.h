// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BC_AI.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UBC_AI : public UInterface
{
	GENERATED_BODY()
};

/** Interface for AI controlled pawns. */
class BASECRAFT_API IBC_AI
{
	GENERATED_BODY()

public:
	/** 
	 * Rotates to target based on delta time.
	 * 
	 * @param Target the actor to face.
	 * 
	 * @param DeltaTime delta time.
	 * 
	 * @return true if rotation to target difference is under threshold.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category="BaseCraft|AI")
	virtual bool RotateToTarget(AActor* Target, float DeltaTime) { return false; }
	
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category="BaseCraft|AI|Combat")
	virtual void QuickAttack() {};
	
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category="BaseCraft|AI|Combat")
	virtual bool IsAttacking() { return false; }

	UFUNCTION(BlueprintCallable, BlueprintCallable, Category="BaseCraft|AI|Combat")
	virtual bool IsTakingDamage() { return false; }
	
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category="BaseCraft|AI|Combat")
	virtual void SetCombatTarget(AActor* Target) {};
	
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category="BaseCraft|AI|Combat")
	virtual AActor* GetCombatTarget() const { return nullptr;}
	
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category="BaseCraft|AI|Navigation")
	virtual AActor* GetNextPatrolTarget() { return nullptr; }
};

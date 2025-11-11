// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BC_DamageableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UBC_DamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASECRAFT_API IBC_DamageableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BaseCraft|Interfaces|Damageable")
	void TakeDamage(const FVector& ImpactPoint, float Damage);
};

// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BC_Attributes.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UBC_Attributes : public UInterface
{
	GENERATED_BODY()
};

class BASECRAFT_API IBC_Attributes
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="BaseCraft|Attributes")
	virtual float GetHealth() const { return 100.0f; }
	
	UFUNCTION(BlueprintCallable, Category="BaseCraft|Attributes")
	virtual float GetHealthPercent() const { return 1.0f;}
	
	UFUNCTION(BlueprintCallable, Category="BaseCraft|Attributes")
	virtual bool IsAlive() const { return true; };	
};

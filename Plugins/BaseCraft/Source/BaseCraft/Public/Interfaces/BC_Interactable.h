// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BC_Interactable.generated.h"

UINTERFACE()
class UBC_Interactable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors that need to be interacted with
 */
class BASECRAFT_API IBC_Interactable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void Interact(AActor* InstigatorActor);
	
};

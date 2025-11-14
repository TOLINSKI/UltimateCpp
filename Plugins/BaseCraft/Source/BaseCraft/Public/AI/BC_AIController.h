// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BC_AIController.generated.h"

class UStateTreeAIComponent;

UCLASS()
class BASECRAFT_API ABC_AIController : public AAIController
{
	GENERATED_BODY()

public:
	ABC_AIController();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseCraft|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStateTreeAIComponent> StateTreeAI;

public:
	FORCEINLINE UStateTreeAIComponent* GetStateTreeAI() { return StateTreeAI; }
};

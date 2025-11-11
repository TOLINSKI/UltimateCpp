// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/BC_AttackerInterface.h"
#include "Interfaces/BC_DamageableInterface.h"
#include "BC_Enemy.generated.h"

UCLASS(ClassGroup=(BaseCraft))
class BASECRAFT_API ABC_Enemy : public ACharacter, public IBC_AttackerInterface, public IBC_DamageableInterface
{
	GENERATED_BODY()

public:
	ABC_Enemy();
};

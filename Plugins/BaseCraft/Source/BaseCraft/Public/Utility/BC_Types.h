// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBC_EnemyState : uint8
{
	EES_None UMETA(DisplayName="None"),
	EES_Idle UMETA(DisplayName="Idle"),
	EES_Patrol UMETA(DisplayName="Patrol"),
	EES_Combat UMETA(DisplayName="Combat")
};
// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBC_AIState : uint8
{
	EES_None UMETA(DisplayName="None"),
	EES_Idle UMETA(DisplayName="Idle"),
	EES_Patrol UMETA(DisplayName="Patrol"),
	EES_Combat UMETA(DisplayName="Combat")
};

UENUM(BlueprintType)
enum class EBC_WeaponType : uint8
{
	EWT_None UMETA(DisplayName="None"),
	EWT_GreatSword UMETA(DisplayName="GreatSword"),
	EWT_ShortSword UMETA(DisplayName="ShortSword"),
	EWT_Dagger UMETA(DisplayName="Dagger")
};
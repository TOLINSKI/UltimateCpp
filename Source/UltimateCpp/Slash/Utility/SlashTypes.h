// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_None UMETA(DisplayName = "None"),
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_Equipped UMETA(DisplayName = "Equipped"),
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_None UMETA(DisplayName = "None"),
	EAS_Unoccupied UMETA(DisplayName = "Unocuppied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Equipping UMETA(DisplayName = "Equipping"),
	EAS_TakingDamage UMETA(DisplayName = "Taking Damage"),
	EAS_Rolling UMETA(DisplayName = "Rolling")
};

UENUM(BlueprintType)
enum class EAttackQuickState : uint8
{
	EQS_None UMETA(DisplayName = "None"),
	EQS_Combo UMETA(DisplayName = "Combo"),
};

UENUM(BlueprintType)
enum class ESlashWeaponType : uint8
{
	EWT_None UMETA(DisplayName = "None"),
	EWT_GreatSword UMETA(DisplayName = "GreatSword"),
	EWT_ShortSword UMETA(DisplayName = "ShortSword"),
	EWT_Dagger UMETA(DisplayName = "Dagger")
};


UENUM(BlueprintType)
enum class ESlashCombatState : uint8
{
	ESC_None UMETA(DisplayName = "None"),
	ESC_Normal UMETA(DisplayName = "Normal"),
	ESC_Invulnerable UMETA(DisplayName = "Invulnerable")
};
// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BC_AttackerInterface.generated.h"

UINTERFACE()
class UBC_AttackerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASECRAFT_API IBC_AttackerInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BaseCraft|Interfaces|Attacker")
	UObject* GetWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BaseCraft|Interfaces|Attacker")
	void SetComboWindowActive(bool bComboWindowActive);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BaseCraft|Interfaces|Attacker")
	void SetAttackBufferWindowActive(bool bAttackBufferWindowActive);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BaseCraft|Interfaces|Attacker")
	void QuickAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BaseCraft|Interfaces|Attacker")
	void HeavyAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BaseCraft|Interfaces|Attacker")
	void EquipWeapon(UObject* Weapon);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BaseCraft|Interfaces|Attacker")
	void UnequipWeapon();	
};

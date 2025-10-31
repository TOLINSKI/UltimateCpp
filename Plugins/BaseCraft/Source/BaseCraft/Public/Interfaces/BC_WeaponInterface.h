// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BC_WeaponInterface.generated.h"

class UMeshComponent;

UINTERFACE()
class UBC_WeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASECRAFT_API IBC_WeaponInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="BaseCraft|Weapon")
	bool DoAttackTrace(FHitResult& HitResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="BaseCraft|Weapon")
	void EnableAttackCollision();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="BaseCraft|Weapon")
	void DisableAttackCollision();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="BaseCraft|Weapon")
	void Attach(USceneComponent* Parent, FName SocketName);
};


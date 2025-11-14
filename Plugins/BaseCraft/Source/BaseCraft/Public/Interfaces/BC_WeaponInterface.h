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
	bool DoAttackTrace(FHitResult& OutHit, const TArray<AActor*>& ActorsToIgnore);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="BaseCraft|Weapon")
	void BeginAttackTracing();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="BaseCraft|Weapon")
	void EndAttackTracing();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="BaseCraft|Weapon")
	void Attach(USceneComponent* Parent, FName SocketName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="BaseCraft|Weapon")
	void SetWeaponTrailActive(bool bWeaponTrailActive);	
};


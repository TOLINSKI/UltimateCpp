// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combat/BC_Weapon.h"
#include "BC_CapsuleWeapon.generated.h"

UCLASS(ClassGroup=(BaseCraft))
class BASECRAFT_API ABC_CapsuleWeapon : public ABC_Weapon
{
	GENERATED_BODY()

public:
	ABC_CapsuleWeapon();

#if WITH_EDITORONLY_DATA
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

// Base Craft Interface
// ====================
public:
	virtual bool DoAttackTrace_Implementation(FHitResult& OutHit, const TArray<AActor*>& ActorsToIgnore) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components", meta = (AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> TraceStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components", meta = (AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> TraceEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon", meta = (AllowPrivateAccess="true"))
	float AttackTraceRadius;
	
#if WITH_EDITORONLY_DATA
	// This capsule component is created only in editor and will not be present in a packaged game
	UPROPERTY()
	TObjectPtr<UCapsuleComponent> AttackCapsulePreview; 
#endif

	//~ Begin Debug 
	/** Returns the radius step from trace-start to trace-end. */
	FVector GetTraceRadiusStep() const;
	float GetTraceCapsuleHalfHeight() const;
	FQuat GetTraceCapsuleRotation() const;
	
#if WITH_EDITOR
	void DrawDebugAttackCapsule(const bool bIsHit, const FHitResult& HitResult) const;
#endif
	//~ End Debug 
};

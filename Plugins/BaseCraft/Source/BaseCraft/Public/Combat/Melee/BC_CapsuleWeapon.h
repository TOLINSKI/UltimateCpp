// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combat/BC_MeleeWeapon.h"
#include "BC_CapsuleWeapon.generated.h"

UCLASS(ClassGroup=(BaseCraft))
class BASECRAFT_API ABC_CapsuleWeapon : public ABC_MeleeWeapon
{
	GENERATED_BODY()

public:
	ABC_CapsuleWeapon();

#if WITH_EDITORONLY_DATA
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

protected:
	virtual void BeginPlay() override;

// Base Craft Interface
// ====================
public:
	virtual bool DoAttackTrace_Implementation(FHitResult& HitResult, const TArray<AActor*>& ActorsToIgnore) override;

private:
	//~ Begin Debug 
	/** Returns the radius step from trace-start to trace-end. */
	FVector GetTraceRadiusStep() const;
	float GetTraceCapsuleHalfHeight() const;
	FQuat GetTraceCapsuleRotation() const;
	
#if WITH_EDITOR
	void DrawDebugAttackCapsule(const bool bIsHit, const FHitResult& HitResult) const;
#endif
	//~ End Debug 
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<USceneComponent> TraceStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<USceneComponent> TraceEnd;

#if WITH_EDITORONLY_DATA
	// This capsule component is created only in editor and will not be present in a packaged game
	UPROPERTY()
	TObjectPtr<UCapsuleComponent> AttackCapsulePreview; 
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	float AttackTraceRadius;
};

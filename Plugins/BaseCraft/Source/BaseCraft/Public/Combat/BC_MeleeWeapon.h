// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/BC_WeaponInterface.h"
#include "BC_MeleeWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBC_OnMeleeWeaponTraceHit, const FHitResult&, HitResult);

class UCapsuleComponent;

UCLASS()
class BASECRAFT_API ABC_MeleeWeapon : public AActor, public IBC_WeaponInterface
{
	GENERATED_BODY()

// UE Interface
// ============
public:
	ABC_MeleeWeapon();

	virtual void Tick(float DeltaTime) override;

#if WITH_EDITORONLY_DATA
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	
private:
	
	TArray<FHitResult> Hits;

	bool bAttackEnabled;

	void DrawDebugAttackCapsule(const FColor& Color);

	/** Returns the radius step from trace-start to trace-end. */
	FVector GetTraceRadiusStep();

	// Debug:
	float GetTraceCapsuleHalfHeight();
	FQuat GetTraceCapsuleRotation();
	
protected:
	virtual void BeginPlay() override;
	
// Base Craft Interface
// ====================
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<USceneComponent> TraceStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<USceneComponent> TraceEnd;

#if WITH_EDITORONLY_DATA
	// This capsule component is created only in editor and will not be present in a packaged game
	UPROPERTY()
	TObjectPtr<UCapsuleComponent> AttackCapsulePreview; 
#endif
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BaseCraft")
	TWeakObjectPtr<APawn> OwnerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	float AttackTraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	TEnumAsByte<ECollisionChannel> AttackTraceChannel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	bool bAttackTraceIgnoreOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon|Debug")
	bool bDrawDebugAttackTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon|Debug", meta=(EditCondition="bDrawDebugAttackTrace", Units="s"))
	float DrawDebugTime;
	
public:
	UPROPERTY(BlueprintAssignable, Category="BaseCraft|Weapon|Events")
	FBC_OnMeleeWeaponTraceHit OnMeleeWeaponHit;
	
	void SetOwnerPawn(APawn* Pawn) { OwnerPawn = Pawn; }

	virtual void Attach_Implementation(USceneComponent* Parent, FName SocketName) override;
	virtual void EnableAttackCollision_Implementation() override;
	virtual void DisableAttackCollision_Implementation() override;
	virtual bool DoAttackTrace_Implementation(FHitResult& HitResult) override;
};

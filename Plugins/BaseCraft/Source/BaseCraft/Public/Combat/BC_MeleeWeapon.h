// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/BC_WeaponInterface.h"
#include "BC_MeleeWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBC_OnMeleeWeaponTraceHit, const FHitResult&, HitResult);

class UCapsuleComponent;

UCLASS(ClassGroup=(BaseCraft))
class BASECRAFT_API ABC_MeleeWeapon : public AActor, public IBC_WeaponInterface
{
	GENERATED_BODY()

// UE Interface
// ============
public:
	ABC_MeleeWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
// Base Craft Interface
// ====================
public:
	virtual void Attach_Implementation(USceneComponent* Parent, FName SocketName) override;
	virtual void BeginAttackTracing_Implementation() override final;
	virtual void EndAttackTracing_Implementation() override final;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	TEnumAsByte<ECollisionChannel> AttackTraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	float BaseDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	bool bAttackTraceIgnoreOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	bool bHitOncePerSwing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon|Debug")
	bool bDrawDebugAttackTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon|Debug", meta=(EditCondition="bDrawDebugAttackTrace", Units="s"))
	float DrawDebugTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon|Debug", meta=(EditCondition="bDrawDebugAttackTrace"))
	bool bDebugHitsSeparate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon|Debug", meta=(EditCondition="bDrawDebugAttackTrace && bDebugHitsSeparate", Units="s"))
	float DrawDebugHitsTime;

	virtual void TickAttackLogic();
	
private:
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> CurrentHitActors;

	void MakeActorsToIgnore(TArray<AActor*>& OutActorsToIgnore);
	
public:
	static ABC_MeleeWeapon* CreateWeapon(APawn* OwnerPawn, TSubclassOf<ABC_MeleeWeapon> WeaponClass);
	
	UPROPERTY(BlueprintAssignable, Category="BaseCraft|Weapon|Events")
	FBC_OnMeleeWeaponTraceHit OnMeleeWeaponHit;
};

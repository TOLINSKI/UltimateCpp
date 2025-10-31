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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<UCapsuleComponent> AttackCapsule; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	bool bCapsuleVisibleInEditor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BaseCraft")
	TWeakObjectPtr<APawn> OwnerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	bool bAttackcCapsuleIngoreOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	TEnumAsByte<ECollisionChannel> AttackTraceChannel;
	
	UFUNCTION()
	virtual void OnAttackCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
public:
	UPROPERTY(BlueprintAssignable, Category="BaseCraft|Weapon|Events")
	FBC_OnMeleeWeaponTraceHit OnMeleeWeaponHit;
	
	
	void SetOwnerPawn(APawn* Pawn) { OwnerPawn = Pawn; }

	virtual void Attach_Implementation(USceneComponent* Parent, FName SocketName) override;
	virtual void EnableAttackCollision_Implementation() override;
	virtual void DisableAttackCollision_Implementation() override;
	virtual bool DoAttackTrace_Implementation(FHitResult& HitResult) override;
};

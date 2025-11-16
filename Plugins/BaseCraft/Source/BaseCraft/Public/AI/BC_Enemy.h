// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/BC_Character.h"
#include "Utility/BC_Types.h"
#include "Interfaces/BC_AI.h"
#include "BC_Enemy.generated.h"

class UWidgetComponent;
class UBC_AttributeComponent;
class UAIPerceptionComponent;
class UStateTreeAIComponent;
struct FAIStimulus;
class UBC_HitStopComponent;

UCLASS(ClassGroup=(BaseCraft))
class BASECRAFT_API ABC_Enemy : public ABC_Character, public IBC_AI
{
	GENERATED_BODY()

public:
	ABC_Enemy();

protected:
	virtual void BeginPlay() override;

// Base Craft Interface
public:
	//~ Begin Damageable Interface
	virtual void TakeDamage_Implementation(AActor* Causer, float Damage, const FHitResult& Hit) override;
	//~ End Damageable interface
	
	//~ Begin AI Interface
	virtual bool RotateToTarget(AActor* Target, float DeltaTime) override;
	virtual bool IsAttacking() override;
	virtual void QuickAttack() override;
	virtual AActor* GetCombatTarget() const override { return CombatTarget.Get();}
	virtual void SetCombatTarget(AActor* NewCombatTarget) override;
	virtual AActor* GetNextPatrolTarget() override;
	virtual bool IsTakingDamage() override;
	//~ End AI Interface

	void SendStateTreeEvent(const FName& GamePlayTagName);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components", meta = (AllowPrivateAccess="true"))
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components", meta = (AllowPrivateAccess="true"))
	TObjectPtr<UAIPerceptionComponent> AIPerception; 
	
protected:
	//~ Begin Navigation
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="BaseCraft|AI|Patrol", meta = (AllowPrivateAccess="true"))
	TArray<TObjectPtr<AActor>> PatrolTargets;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="BaseCraft|AI|Patrol", meta = (AllowPrivateAccess="true"))
	TObjectPtr<AActor> PatrolTarget;
	//~ End Navigation
	
	//~ Begin Satate
	virtual void HandleDeath() override;

	UFUNCTION()
	virtual void OnAIPerceptionTargetUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|AI")
	float LifeSpan;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BaseCraft|AI|State", meta = (AllowPrivateAccess="true"))
	EBC_EnemyState EnemyState;
	//~ End State
	
	//~ Begin Combat
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="BaseCraft|AI|Combat", meta = (AllowPrivateAccess="true"))
	TWeakObjectPtr<AActor> CombatTarget;
	
	virtual void AcquireCombatTarget(AActor* NewCombatTarget);
	virtual void LooseCombatTarget();
	
	/** Under this radius, AI Perception will not affect loosing the target. I.e. even if AI Perception does not sense the target, the enemy will still be aware of it. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|AI|Combat")
	float CombatSensingRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|AI|Combat|FaceCombatTarget")
	float RotateToTargetSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|AI|Combat|FaceCombatTarget")
	float RotateToTargetThreshold;
	//~ End Combat
	
public:
	UWidgetComponent* GetHealthBar() const { return HealthBar; }
	UAIPerceptionComponent* GetAIPerception() const { return AIPerception; }
	
	float GetFaceCombatTargetSpeed() const { return RotateToTargetSpeed; }
	float GetFaceCombatTargetThreshold() const { return RotateToTargetThreshold; }
	
	UFUNCTION(BlueprintCallable, Category="BaseCraft|AI")
	void SetShowHealthBar(bool bShowHealthBar);
};

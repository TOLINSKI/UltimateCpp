// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/BC_Character.h"
#include "BC_Enemy.generated.h"

class UWidgetComponent;
class UBC_AttributeComponent;
class UAIPerceptionComponent;
class UStateTreeAIComponent;
struct FAIStimulus;
class UBC_HitStopComponent;

UCLASS(ClassGroup=(BaseCraft))
class BASECRAFT_API ABC_Enemy : public ABC_Character
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

	void SendStateTreeEvent(const FName& GamePlayTagName);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<UAIPerceptionComponent> AIPerception; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft")
	float LifeSpan;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="BaseCraft|AI", meta=(AllowPrivateAccess="true"))
	TArray<AActor*> PatrolTargets;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="BaseCraft|AI", meta=(AllowPrivateAccess="true"))
	TObjectPtr<AActor> PatrolTarget;
	
	virtual void HandleDeath() override;

	UFUNCTION()
	virtual void OnAIPerceptionTargetUpdated(AActor* Actor, FAIStimulus Stimulus);

public:
	
	UFUNCTION(BlueprintCallable, Category="BaseCraft|AI")
	virtual AActor* GetNextPatrolTarget();
	
	UFUNCTION(BlueprintCallable, Category="BaseCraft|AI")
	void SetShowHealthBar(bool bShowHealthBar);
	
};

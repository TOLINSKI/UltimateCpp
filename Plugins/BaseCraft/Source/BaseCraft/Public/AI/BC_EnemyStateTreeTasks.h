// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "StateTreeTaskBase.h"
#include "StateTreeConditionBase.h"

#include "BC_EnemyStateTreeTasks.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Instance data for BC Face Target Task */
USTRUCT()
struct FBC_StateTreeFaceTargetInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AActor> Actor;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<AActor> Target;
};

/** StateTree task to face an AI-Controlled Pawn towards an Actor */
USTRUCT(meta=(DisplayName="BC Face Target", Category="BaseCraft"))
struct FBC_StateTreeFaceTargetTask: public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	/* Ensure we're using the correct instance data struct */
	using FInstanceDataType = FBC_StateTreeFaceTargetInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	
#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif // WITH_EDITOR
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Instance data for BC Get Combat Target Info Task */
USTRUCT()
struct FBC_StateTreeGetTargetInfoInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AActor> Actor {nullptr};
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<AActor> Target {nullptr};
	
	UPROPERTY(VisibleAnywhere, Category = Parameters)
	FVector TargetLocation {FVector::ZeroVector};

	UPROPERTY(VisibleAnywhere, Category = Parameters)
	float DistanceToTarget {INFINITY};
	
	UPROPERTY(VisibleAnywhere, Category = Parameters)
	float TargetHealth{100.0f};
	
	UPROPERTY(VisibleAnywhere, Category = Parameters)
	float TargetHealthPercent{1.0f};	
	
	UPROPERTY(VisibleAnywhere, Category = Parameters)
	bool bTargetIsAlive{true};
	
	UPROPERTY(VisibleAnywhere, Category = Events)
	FStateTreeDelegateDispatcher TargetDied;
};

/** StateTree task to get info about a target. */
USTRUCT(meta=(DisplayName="BC Get Target Info", Category="BaseCraft"))
struct FBC_StateTreeGetTargetInfoTask: public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	/* Ensure we're using the correct instance data struct */
	using FInstanceDataType = FBC_StateTreeGetTargetInfoInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	
#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif // WITH_EDITOR
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Instance data for BC Quick Attack Task */
USTRUCT()
struct FBC_StateTreeQuickAttackInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AActor> Actor {nullptr};
};

/** StateTree task to get info about a target. */
USTRUCT(meta=(DisplayName="BC Quick Attack Task", Category="BaseCraft"))
struct FBC_StateTreeQuickAttackTask: public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	/* Ensure we're using the correct instance data struct */
	using FInstanceDataType = FBC_StateTreeQuickAttackInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	
#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif // WITH_EDITOR
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////
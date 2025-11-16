// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreeConditionBase.h"

#include "StateTreeTutorialUtility.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *  Instance data struct for the Face Towards Actor StateTree task
 */
USTRUCT()
struct FStateTreeGetOverlapActorCountInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AActor> Actor;
	
	UPROPERTY(EditAnywhere, Category = Output)
	int32 OverlapCount { 0 };

	UPROPERTY(VisibleAnywhere, Category = Event)
	FStateTreeDelegateDispatcher OnOverlapActorCountChanged;
};

/**
 *  StateTree task to face an AI-Controlled Pawn towards an Actor
 */
USTRUCT(meta=(DisplayName="Get Overlap Actor Count", Category="Weighted Platform"))
struct FStateTreeGetOverlapActorCountTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	/* Ensure we're using the correct instance data struct */
	using FInstanceDataType = FStateTreeGetOverlapActorCountInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	/** Runs when the owning state is entered */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	
	/** Runs when the owning state is ended */
	// virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif // WITH_EDITOR
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *  Instance data struct for the Face Towards Actor StateTree task
 */
USTRUCT()
struct FStateTreeMoveToPosInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AActor> Actor;
	
	UPROPERTY(EditAnywhere, Category = Input)
	FVector TargetPos { FVector::ZeroVector };

	UPROPERTY(EditAnywhere, Category = Parameter)
	float Speed { 100.0f };

	UPROPERTY(VisibleAnywhere, Category = Parameter)
	bool bHasReachedTarget { false };
};

/**
 *  StateTree task to face an AI-Controlled Pawn towards an Actor
 */
USTRUCT(meta=(DisplayName="Move to Position", Category="Weighted Platform"))
struct FStateTreeMoveToPosTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	/* Ensure we're using the correct instance data struct */
	using FInstanceDataType = FStateTreeMoveToPosInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	/** Runs when the owning state is entered */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	bool CheckReachedTarget(FStateTreeExecutionContext& Context) const;

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	
	/** Runs when the owning state is ended */
	// virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif // WITH_EDITOR
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *  Instance data struct for the Face Towards Actor StateTree task
 */
USTRUCT()
struct FStateTreeSpawnSoundInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AActor> Actor;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<USoundBase> SoundToPlay;

	UPROPERTY(EditAnywhere, Category = Output)
	TObjectPtr<UAudioComponent> SpawnedAudio; 
	
	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bIsAttached { false };

	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bDestroyAtTransition { false };
};

/**
 *  StateTree task to face an AI-Controlled Pawn towards an Actor
 */
USTRUCT(meta=(DisplayName="Spawn Sound", Category="General"))
struct FStateTreeSpawnSoundTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	/* Ensure we're using the correct instance data struct */
	using FInstanceDataType = FStateTreeSpawnSoundInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	/** Runs when the owning state is entered */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
	/** Runs when the owning state is ended */
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif // WITH_EDITOR
};

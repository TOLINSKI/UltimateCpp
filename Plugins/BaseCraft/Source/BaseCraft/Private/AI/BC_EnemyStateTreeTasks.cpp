// Copyright Benski Game Works 2025, All rights reserved.


#include "AI/BC_EnemyStateTreeTasks.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeExecutionTypes.h"
#include "Interfaces/BC_AI.h"
#include "Interfaces/BC_Attributes.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//~ Begin Face Combat Target Task
EStateTreeRunStatus FBC_StateTreeFaceTargetTask::Tick(FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	// get the instance data
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	bool bLookingAtTarget { false };
	
	if (IBC_AI* AI = Cast<IBC_AI>(InstanceData.Actor); InstanceData.Target)
	{
		bLookingAtTarget = AI->RotateToTarget(
			InstanceData.Target, 
			DeltaTime 
			);
	}
	
	return bLookingAtTarget ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Running;
}

FText FBC_StateTreeFaceTargetTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return  FText::FromString("<b>Face Target</b>");
}
//~ End Face Combat Target Task
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
//~ Begin Get Target Info Task
EStateTreeRunStatus FBC_StateTreeGetTargetInfoTask::Tick(FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	// get the instance data
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	// do we have a valid target?
	if (InstanceData.Target)
	{
		InstanceData.TargetLocation = InstanceData.Target->GetActorLocation();
		InstanceData.DistanceToTarget = FVector::Dist(InstanceData.TargetLocation, InstanceData.Actor->GetActorLocation());
		
		if (IBC_Attributes* IAttributes = Cast<IBC_Attributes>(InstanceData.Actor))
		{
			InstanceData.TargetHealth = IAttributes->GetHealth();
			InstanceData.TargetHealthPercent = IAttributes->GetHealthPercent();
			
			// When last frame was alive, but now is dead, broadcast
			if (InstanceData.bTargetIsAlive == true && IAttributes->IsAlive() == false)
				Context.BroadcastDelegate(InstanceData.TargetDied);
			
			InstanceData.bTargetIsAlive = IAttributes->IsAlive();
		}
	}

	return EStateTreeRunStatus::Running;
}

FText FBC_StateTreeGetTargetInfoTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return  FText::FromString("<b>Get Target Info</b>");
}
//~ End Get Target Info Task
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
//~ Begin Quick Attack Task
EStateTreeRunStatus FBC_StateTreeQuickAttackTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	if (Transition.ChangeType == EStateTreeStateChangeType::Changed)
	{
		// get the instance data
		FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

		// set the character's max ground speed
		if (IBC_AI* AI = Cast<IBC_AI>(InstanceData.Actor))
		{
			AI->QuickAttack();
		}
	}
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FBC_StateTreeQuickAttackTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	// get the instance data
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (IBC_AI* AI = Cast<IBC_AI>(InstanceData.Actor))
	{
		if (AI->IsAttacking() == false)
		{
			return EStateTreeRunStatus::Succeeded;
		}
	}
	
	return EStateTreeRunStatus::Running;
}

FText FBC_StateTreeQuickAttackTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
                                                   const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return FText::FromString("<b>Quick Attack</b>");
}
//~ End Quick Attack Task
//////////////////////////////////////////////////////////////////////////////////////////////////


// Copyright Benski Game Works 2025, All rights reserved.


#include "StateTreeTutorialUtility.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeExecutionTypes.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "kismet/GameplayStatics.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////

EStateTreeRunStatus FStateTreeGetOverlapActorCountTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	// have we transitioned from another state?
	if (Transition.ChangeType == EStateTreeStateChangeType::Changed)
	{
		// get the instance data
		FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

		if (UBoxComponent* Box = InstanceData.Actor->GetComponentByClass<UBoxComponent>())
		{
			TArray<AActor*> OverlappingActors;
			Box->GetOverlappingActors(OverlappingActors);
			InstanceData.OverlapCount = OverlappingActors.Num();
		}
	}
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FStateTreeGetOverlapActorCountTask::Tick(FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	// get the instance data
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (UBoxComponent* Box = InstanceData.Actor->GetComponentByClass<UBoxComponent>())
	{
		TArray<AActor*> OverlappingActors;
		Box->GetOverlappingActors(OverlappingActors);
		int32 NewOverlapCount = OverlappingActors.Num();

		if (NewOverlapCount != InstanceData.OverlapCount)
			Context.BroadcastDelegate(InstanceData.OnOverlapActorCountChanged);
			
		InstanceData.OverlapCount = OverlappingActors.Num();
	}
	
	return EStateTreeRunStatus::Running;
}

FText FStateTreeGetOverlapActorCountTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
                                                         const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return FText::FromString("<b>Get Overlapping Actor Count</b>");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

EStateTreeRunStatus FStateTreeMoveToPosTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	InstanceData.bHasReachedTarget = CheckReachedTarget(Context);

	if (InstanceData.bHasReachedTarget)
		return EStateTreeRunStatus::Succeeded;

	return EStateTreeRunStatus::Running;
}

bool FStateTreeMoveToPosTask::CheckReachedTarget(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	AActor* Actor = InstanceData.Actor;
	FVector ActorLocation = Actor->GetActorLocation();
	FVector TargetLocation = InstanceData.TargetPos;
	float Speed = InstanceData.Speed;

	return (Actor->GetActorLocation()-TargetLocation).IsNearlyZero();
}

EStateTreeRunStatus FStateTreeMoveToPosTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	AActor* Actor = InstanceData.Actor;
	FVector ActorLocation = Actor->GetActorLocation();
	FVector TargetLocation = InstanceData.TargetPos;
	float Speed = InstanceData.Speed;
	
	Actor->SetActorLocation(
	FMath::VInterpConstantTo(
		ActorLocation,
		TargetLocation,
		DeltaTime,
		Speed)
	, true);	
	
	InstanceData.bHasReachedTarget = CheckReachedTarget(Context);

	if (InstanceData.bHasReachedTarget)
		return EStateTreeRunStatus::Succeeded;
	
	return EStateTreeRunStatus::Running;
}

FText FStateTreeMoveToPosTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return FText::FromString("<b>Move to Position</b>");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

EStateTreeRunStatus FStateTreeSpawnSoundTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	AActor* Actor = InstanceData.Actor;
	USoundBase* Sound = InstanceData.SoundToPlay;
	
	if (!Sound)
		return EStateTreeRunStatus::Running;

	if (InstanceData.bIsAttached)
	{
		InstanceData.SpawnedAudio = UGameplayStatics::SpawnSoundAttached(Sound, Actor->GetRootComponent());
	}
	else
	{
		InstanceData.SpawnedAudio = UGameplayStatics::SpawnSoundAtLocation(Actor, Sound, Actor->GetActorLocation());
	}
	
	return EStateTreeRunStatus::Running;
}

void FStateTreeSpawnSoundTask::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (InstanceData.bDestroyAtTransition && InstanceData.SpawnedAudio)
	{
		InstanceData.SpawnedAudio->Stop();
		InstanceData.SpawnedAudio = nullptr;
	}
}

FText FStateTreeSpawnSoundTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return FText::FromString("<b>Spawn Sound</b>");
}

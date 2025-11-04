// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BC_AnimNotify_EndAttackTracing.generated.h"

/**
 * 
 */
UCLASS()
class BASECRAFT_API UBC_AnimNotify_EndAttackTracing : public UAnimNotify
{
	GENERATED_BODY()

	/** Perform the Anim Notify */
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	/** Get the notify name */
	virtual FString GetNotifyName_Implementation() const override;
};

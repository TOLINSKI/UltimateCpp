// Copyright Benski Game Works 2025, All rights reserved.


#include "Animation/AnimNotifies/BC_AnimNotifyState_AttackBufferWindow.h"
#include "Interfaces/BC_AttackerInterface.h"

void UBC_AnimNotifyState_AttackBufferWindow::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                                         UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Attacker = MeshComp->GetOwner(); Attacker->Implements<UBC_AttackerInterface>())
	{
		IBC_AttackerInterface::Execute_SetAttackBufferWindowActive(Attacker, true);
	}
}

void UBC_AnimNotifyState_AttackBufferWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Attacker = MeshComp->GetOwner(); Attacker->Implements<UBC_AttackerInterface>())
	{
		IBC_AttackerInterface::Execute_SetAttackBufferWindowActive(Attacker, false);
	}
}

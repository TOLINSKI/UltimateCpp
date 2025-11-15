// Copyright Benski Game Works 2025, All rights reserved.


#include "Animation/AnimNotifies/BC_AnimNotifyState_ComboWindow.h"
#include "Interfaces/BC_Attacker.h"

void UBC_AnimNotifyState_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Attacker = MeshComp->GetOwner(); Attacker->Implements<UBC_Attacker>())
	{
		IBC_Attacker::Execute_SetComboWindowActive(Attacker, true);
	}
}

void UBC_AnimNotifyState_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Attacker = MeshComp->GetOwner(); Attacker->Implements<UBC_Attacker>())
	{
		IBC_Attacker::Execute_SetComboWindowActive(Attacker, false);
	}
}

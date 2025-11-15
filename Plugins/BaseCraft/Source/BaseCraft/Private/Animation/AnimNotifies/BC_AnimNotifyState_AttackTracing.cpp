// Copyright Benski Game Works 2025, All rights reserved.


#include "Animation/AnimNotifies/BC_AnimNotifyState_AttackTracing.h"
#include "Interfaces/BC_Attacker.h"
#include "Interfaces/BC_WeaponInterface.h"

void UBC_AnimNotifyState_AttackTracing::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Attacker = MeshComp->GetOwner(); Attacker->Implements<UBC_Attacker>())
	{
		if (UObject* Weapon = IBC_Attacker::Execute_GetWeapon(Attacker); Weapon->Implements<UBC_WeaponInterface>())
		{
			IBC_WeaponInterface::Execute_BeginAttackTracing(Weapon);
		}
	}
}

void UBC_AnimNotifyState_AttackTracing::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Attacker = MeshComp->GetOwner(); Attacker->Implements<UBC_Attacker>())
	{
		if (UObject* Weapon = IBC_Attacker::Execute_GetWeapon(Attacker); Weapon->Implements<UBC_WeaponInterface>())
		{
			IBC_WeaponInterface::Execute_EndAttackTracing(Weapon);
		}
	}
}

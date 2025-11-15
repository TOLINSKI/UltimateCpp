// Copyright Benski Game Works 2025, All rights reserved.


#include "AnimNotifyState_WeaponTrail.h"
#include "Interfaces/BC_Attacker.h"
#include "Interfaces/BC_WeaponInterface.h"

void UAnimNotifyState_WeaponTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Attacker = MeshComp->GetOwner(); Attacker->Implements<UBC_Attacker>())
	{
		if (UObject* Weapon = IBC_Attacker::Execute_GetWeapon(Attacker); Weapon->Implements<UBC_WeaponInterface>())
		{
			IBC_WeaponInterface::Execute_SetWeaponTrailActive(Weapon, true);
		}
	}
}

void UAnimNotifyState_WeaponTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Attacker = MeshComp->GetOwner(); Attacker->Implements<UBC_Attacker>())
	{
		if (UObject* Weapon = IBC_Attacker::Execute_GetWeapon(Attacker); Weapon->Implements<UBC_WeaponInterface>())
		{
			IBC_WeaponInterface::Execute_SetWeaponTrailActive(Weapon, false);
		}
	}
}

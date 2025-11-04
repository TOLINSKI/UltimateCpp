// Copyright Benski Game Works 2025, All rights reserved.


#include "Animation/AnimNotifies/BC_AnimNotify_BeginAttackTracing.h"
#include "Interfaces/BC_AttackerInterface.h"
#include "Interfaces/BC_WeaponInterface.h"

void UBC_AnimNotify_BeginAttackTracing::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Attacker = MeshComp->GetOwner(); Attacker && Attacker->Implements<UBC_AttackerInterface>())
	{
		if (UObject* Weapon = IBC_AttackerInterface::Execute_GetWeapon(Attacker); Weapon->Implements<UBC_WeaponInterface>())
		{
			IBC_WeaponInterface::Execute_BeginAttackTracing(Weapon);
		}
	}
}

FString UBC_AnimNotify_BeginAttackTracing::GetNotifyName_Implementation() const
{
	return FString("Begin Attack Tracing");
}

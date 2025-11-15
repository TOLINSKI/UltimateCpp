// Copyright Benski Game Works 2025, All rights reserved.


#include "AnimNotify_WeaponSheathed.h"

#include "SlashCharacter.h"

void UAnimNotify_WeaponSheathed::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	if (ASlashCharacter* Character = Cast<ASlashCharacter>(MeshComp->GetOwner()))
	{
		if (UObject* Weapon = IBC_Attacker::Execute_GetWeapon(Character))
		{
			IBC_WeaponInterface::Execute_Attach(Weapon, Character->GetMesh(), Character->GetBackSocketName());
		}
	}
}

FString UAnimNotify_WeaponSheathed::GetNotifyName_Implementation() const
{
	return FString("Weapon Sheathed");
}

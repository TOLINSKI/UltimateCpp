// Copyright Benski Game Works 2025, All rights reserved.


#include "AnimNotify_WeaponSheathed.h"

#include "SlashCharacter.h"
#include "Items/Weapons/SlashWeapon.h"

void UAnimNotify_WeaponSheathed::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	if (ASlashCharacter* Character = Cast<ASlashCharacter>(MeshComp->GetOwner()))
	{
		if (ASlashWeapon* Weapon = Character->GetWeapon())
		{
			Weapon->AttachToBack(Character);
		}
	}
}

FString UAnimNotify_WeaponSheathed::GetNotifyName_Implementation() const
{
	return FString("Weapon Sheathed");
}

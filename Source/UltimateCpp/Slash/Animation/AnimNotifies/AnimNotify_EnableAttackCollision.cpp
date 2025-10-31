// Copyright Benski Game Works 2025, All rights reserved.


#include "AnimNotify_EnableAttackCollision.h"
#include "Slash/SlashCharacter.h"
#include "Interfaces/BC_WeaponInterface.h"

void UAnimNotify_EnableAttackCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               const FAnimNotifyEventReference& EventReference)
{
	if (ASlashCharacter* Character = Cast<ASlashCharacter>(MeshComp->GetOwner()))
	{
		AActor* Weapon = Character->GetCurrentWeapon();
		if (Weapon && Weapon->Implements<UBC_WeaponInterface>())
		{
			IBC_WeaponInterface::Execute_EnableAttackCollision(Weapon);
		}
	}
}

FString UAnimNotify_EnableAttackCollision::GetNotifyName_Implementation() const
{
	return FString("Enable Attack Collision");
}

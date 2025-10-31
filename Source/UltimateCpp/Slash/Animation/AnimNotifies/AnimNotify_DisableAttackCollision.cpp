// Copyright Benski Game Works 2025, All rights reserved.


#include "AnimNotify_DisableAttackCollision.h"
#include "Slash/SlashCharacter.h"
#include "Interfaces/BC_WeaponInterface.h"

void UAnimNotify_DisableAttackCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                const FAnimNotifyEventReference& EventReference)
{
	if (ASlashCharacter* Character = Cast<ASlashCharacter>(MeshComp->GetOwner()))
	{
		AActor* Weapon = Character->GetCurrentWeapon();
		if (Weapon && Weapon->Implements<UBC_WeaponInterface>())
		{
			IBC_WeaponInterface::Execute_DisableAttackCollision(Weapon);
		}
	}
}

FString UAnimNotify_DisableAttackCollision::GetNotifyName_Implementation() const
{
	return FString("Disable Attack Collision");
}

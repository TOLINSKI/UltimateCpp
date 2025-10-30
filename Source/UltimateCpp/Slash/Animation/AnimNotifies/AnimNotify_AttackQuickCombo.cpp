// Copyright Benski Game Works 2025, All rights reserved.


#include "AnimNotify_AttackQuickCombo.h"
#include "Slash/SlashCharacter.h"

void UAnimNotify_AttackQuickCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (ASlashCharacter* Character = Cast<ASlashCharacter>(MeshComp->GetOwner()))
	{
		Character->SetAttackQuickCombo(true);
	}
}

FString UAnimNotify_AttackQuickCombo::GetNotifyName_Implementation() const
{
	return FString("Attack Quick Combo");
}

// Copyright Benski Game Works 2025, All rights reserved.


#include "Components/MontageComponent/BC_CharacterMontageComponent.h"
#include "GameFramework/Character.h"


UBC_CharacterMontageComponent::UBC_CharacterMontageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBC_CharacterMontageComponent::GetMontagesSoftPaths(TArray<FSoftObjectPath>& SoftObjectPaths) const
{
	SoftObjectPaths.Add(HitReactMontage.ToSoftObjectPath());
	SoftObjectPaths.Add(RollMontage.ToSoftObjectPath());
}

void UBC_CharacterMontageComponent::OnMontagesLoaded()
{
	Super::OnMontagesLoaded();

	HitReact = HitReactMontage.Get();
	Roll = RollMontage.Get();
}

void UBC_CharacterMontageComponent::PlayMontage(EBC_MontageType MontageType, FName SectionName)
{
	if (!ensureMsgf(IsMontagesLoaded(), TEXT("Montages must be loaded before using.")))
		return;
	
	UAnimMontage* AnimMontage;

	switch (MontageType)
	{
	case EMT_HitReact:
		AnimMontage = HitReact;
		break;
	case EMT_Roll:
		AnimMontage = Roll;
		break;
	default:
		return;
	}

	if (!AnimMontage)
		return;
			
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(AnimMontage);

			if (!SectionName.IsNone())
				AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
		}
	}
}

void UBC_CharacterMontageComponent::PlayMontageWithEndDelegate(FOnMontageEnded& EndDelegate,
	EBC_MontageType MontageType, FName SectionName)
{
	if (!ensureMsgf(IsMontagesLoaded(), TEXT("Montages must be loaded before playing. Please call LoadMontages() before using.")))
		return;
	
	UAnimMontage* AnimMontage;

	switch (MontageType)
	{
	case EMT_HitReact:
		AnimMontage = HitReact;
		break;
	case EMT_Roll:
		AnimMontage = Roll;
		break;
	default:
		return;
	}

	if (!AnimMontage)
		return;
			
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(AnimMontage);

			if (!SectionName.IsNone())
				AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);

			AnimInstance->Montage_SetEndDelegate(EndDelegate, AnimMontage);
		}
	}
}

bool UBC_CharacterMontageComponent::IsPlayingMontage(EBC_MontageType MontageType)
{
	if (!ensureMsgf(IsMontagesLoaded(), TEXT("Montages must be loaded before using.")))
		return false;
	
	UAnimInstance* AnimInstance { nullptr };
	
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()); Character)
		AnimInstance = Character->GetMesh()->GetAnimInstance();
	
	if (!ensureMsgf(AnimInstance != nullptr, TEXT("Anim instance not found!")))
		return false;
	
	switch (MontageType)
	{
	case EMT_HitReact:
		return AnimInstance->Montage_IsPlaying(HitReact);
	case EMT_Roll:
		return AnimInstance->Montage_IsPlaying(Roll);
	default:
		return false;
	}
}



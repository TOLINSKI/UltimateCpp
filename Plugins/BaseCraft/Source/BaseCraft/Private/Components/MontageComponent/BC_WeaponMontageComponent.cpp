// Copyright Benski Game Works 2025, All rights reserved.


#include "Components/MontageComponent/BC_WeaponMontageComponent.h"
#include "GameFramework/Character.h"

UBC_WeaponMontageComponent::UBC_WeaponMontageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBC_WeaponMontageComponent::GetMontagesSoftPaths(TArray<FSoftObjectPath>& SoftObjectPaths) const
{
	Super::GetMontagesSoftPaths(SoftObjectPaths);
	
	SoftObjectPaths.Add(QuickAttackMontage.ToSoftObjectPath());
	SoftObjectPaths.Add(EquipWeaponMontage.ToSoftObjectPath());
}

void UBC_WeaponMontageComponent::OnMontagesLoaded()
{
	Super::OnMontagesLoaded();
	
	QuickAttack = QuickAttackMontage.Get();
	EquipWeapon = EquipWeaponMontage.Get();
}

void UBC_WeaponMontageComponent::PlayMontage(EBC_MontageType MontageType, FName SectionName)
{
	if (!ensureMsgf(IsMontagesLoaded(), TEXT("Montages must be loaded before playing. Please call LoadMontages() before using.")))
		return;
	
	UAnimMontage* AnimMontage;

	switch (MontageType)
	{
	case EMT_QuickAttack:
		AnimMontage = QuickAttack;
		break;
	case EMT_EquipWeapon:
		AnimMontage = EquipWeapon;
		break;
	default:
		return;
	}

	if (!AnimMontage)
		return;
			
	if (const ACharacter* Character = Cast<ACharacter>(GetOwner()->GetInstigator()))
	{
		if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(AnimMontage);

			if (!SectionName.IsNone())
				AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
		}
	}
}

void UBC_WeaponMontageComponent::PlayMontageWithEndDelegate(FOnMontageEnded& EndDelegate, EBC_MontageType MontageType,
	FName SectionName)
{
	if (!ensureMsgf(IsMontagesLoaded(), TEXT("Montages must be loaded before playing. Please call LoadMontages() before using.")))
		return;
	
	UAnimMontage* AnimMontage;

	switch (MontageType)
	{
	case EMT_QuickAttack:
		AnimMontage = QuickAttack;
		break;
	case EMT_EquipWeapon:
		AnimMontage = EquipWeapon;
		break;
	default:
		return;
	}

	if (!AnimMontage)
		return;
			
	if (const ACharacter* Character = Cast<ACharacter>(GetOwner()->GetInstigator()))
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

bool UBC_WeaponMontageComponent::IsPlayingMontage(EBC_MontageType MontageType)
{
	if (!ensureMsgf(IsMontagesLoaded(), TEXT("Montages must be loaded before using.")))
		return false;
	
	UAnimInstance* AnimInstance { nullptr };
	
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()->GetInstigator()); Character)
		AnimInstance = Character->GetMesh()->GetAnimInstance();
	
	if (!ensureMsgf(AnimInstance != nullptr, TEXT("Anim instance not found!")))
		return false;
	
	switch (MontageType)
	{
	case EMT_EquipWeapon:
		return AnimInstance->Montage_IsPlaying(EquipWeapon);
	case EMT_QuickAttack:
		return AnimInstance->Montage_IsPlaying(QuickAttack);
	default:
		return false;
	}
}


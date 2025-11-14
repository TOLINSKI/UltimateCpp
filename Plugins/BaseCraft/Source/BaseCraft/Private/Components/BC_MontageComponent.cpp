// Copyright Benski Game Works 2025, All rights reserved.


#include "Components/BC_MontageComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

using enum EBC_MontageType;

UBC_MontageComponent::UBC_MontageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	bMontagesLoaded = false;
}

void UBC_MontageComponent::LoadMontages()
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	FStreamableDelegate StreamableDelegate;
	StreamableDelegate.BindWeakLambda(this,
		[this](){
			QuickAttack = QuickAttackMontage.Get();
			HitReact = HitReactMontage.Get();
			EquipWeapon = EquipWeaponMontage.Get();
			
			bMontagesLoaded = true;
		});

	TArray<FSoftObjectPath> SoftObjectPaths;
	SoftObjectPaths.Add(QuickAttackMontage.ToSoftObjectPath());
	SoftObjectPaths.Add(HitReactMontage.ToSoftObjectPath());
	SoftObjectPaths.Add(EquipWeaponMontage.ToSoftObjectPath());
	
	StreamableManager.RequestAsyncLoad(SoftObjectPaths, StreamableDelegate);
}

void UBC_MontageComponent::PlayMontage(EBC_MontageType MontageType, FName SectionName)
{
	if (!ensureMsgf(bMontagesLoaded, TEXT("Montages must be loaded before playing. Please call LoadMontages() before using.")))
		return;
	
	UAnimMontage* AnimMontage;

	switch (MontageType)
	{
	case EMT_QuickAttack:
		AnimMontage = QuickAttack;
		break;
	case EMT_HitReact:
		AnimMontage = HitReact;
		break;
	case EMT_EquipWeapon:
		AnimMontage = EquipWeapon;
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

void UBC_MontageComponent::PlayMontageWithEndDelegate(FOnMontageEnded& EndDelegate, EBC_MontageType MontageType,
	FName SectionName)
{
	if (!ensureMsgf(bMontagesLoaded, TEXT("Montages must be loaded before playing. Please call LoadMontages() before using.")))
		return;
	
	UAnimMontage* AnimMontage;

	switch (MontageType)
	{
	case EMT_QuickAttack:
		AnimMontage = QuickAttack;
		break;
	case EMT_HitReact:
		AnimMontage = HitReact;
		break;
	case EMT_EquipWeapon:
		AnimMontage = EquipWeapon;
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

FName UBC_MontageComponent::GetHitReactMontageSectionName(const FVector& ImpactPoint) const
{
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	const FVector ToImpact = ((ImpactPoint - OwnerLocation)*FVector(1.0f, 1.0f, 0.0f)).GetSafeNormal();
	const FVector Forward = GetOwner()->GetActorForwardVector();
	const float Angle = FMath::Acos(Forward.Dot(ToImpact)); // Always positive!
	const float CrossZ = Forward.Cross(ToImpact).Z; // Positive when to the right of forward
	
	// Angle is always positive, use cross Z to determine sign
	if (Angle <= UE_PI*0.25f)
		return TEXT("FromFront");
	if (Angle >= UE_PI*0.75f)
		return TEXT("FromBack");
	if (CrossZ < 0.0f)
		return TEXT("FromLeft");
	if (CrossZ > 0.0f)
		return TEXT("FromRight");

	return TEXT("FromFront");
}

void UBC_MontageComponent::SetMontage(EBC_MontageType MontageType, TSoftObjectPtr<UAnimMontage> NewMontage)
{
	switch (MontageType)
	{
	case EMT_QuickAttack:
		QuickAttackMontage = NewMontage;
		break;
	case EMT_HitReact:
		HitReactMontage = NewMontage;
		break;
	default:
		return;
	}
}




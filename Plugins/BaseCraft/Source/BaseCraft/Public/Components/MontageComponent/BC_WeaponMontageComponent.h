// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BC_MontageComponent.h"
#include "BC_WeaponMontageComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASECRAFT_API UBC_WeaponMontageComponent : public UBC_MontageComponent
{
	GENERATED_BODY()
	
public:
	UBC_WeaponMontageComponent();
	
	//~ Begin Montage Component Interface 
	virtual void GetMontagesSoftPaths(TArray<FSoftObjectPath>& SoftObjectPaths) const override;
	virtual void OnMontagesLoaded() override;
	virtual void PlayMontage(EBC_MontageType MontageType, FName SectionName = NAME_None) override;
	virtual void PlayMontageWithEndDelegate(FOnMontageEnded& EndDelegate, EBC_MontageType MontageType, FName SectionName = NAME_None) override;
	virtual bool IsPlayingMontage(EBC_MontageType MontageType) override;
	//~ End Montage Component Interface
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BaseCraft|Montages|Combat", meta = (AllowPrivateAccess="true"))
	TSoftObjectPtr<UAnimMontage> QuickAttackMontage;

	UPROPERTY()
	TObjectPtr<UAnimMontage> QuickAttack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BaseCraft|Montages|Combat", meta = (AllowPrivateAccess="true"))
	TSoftObjectPtr<UAnimMontage> EquipWeaponMontage;

	UPROPERTY()
	TObjectPtr<UAnimMontage> EquipWeapon;
};

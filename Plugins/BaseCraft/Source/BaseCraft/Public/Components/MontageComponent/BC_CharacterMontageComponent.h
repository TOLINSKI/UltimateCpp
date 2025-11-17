// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BC_MontageComponent.h"
#include "BC_CharacterMontageComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASECRAFT_API UBC_CharacterMontageComponent : public UBC_MontageComponent
{
	GENERATED_BODY()

public:
	UBC_CharacterMontageComponent();
	
	//~ Begin Montage Component Interface 
	virtual void GetMontagesSoftPaths(TArray<FSoftObjectPath>& SoftObjectPaths) const override;	
	virtual void OnMontagesLoaded() override;
	virtual void PlayMontage(EBC_MontageType MontageType, FName SectionName = NAME_None) override;
	virtual void PlayMontageWithEndDelegate(FOnMontageEnded& EndDelegate, EBC_MontageType MontageType, FName SectionName = NAME_None) override;
	virtual bool IsPlayingMontage(EBC_MontageType MontageType) override;
	//~ End Montage Component Interface 
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BaseCraft|Montages|Combat", meta = (AllowPrivateAccess="true"))
	TSoftObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY()
	TObjectPtr<UAnimMontage> HitReact;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BaseCraft|Montages|Combat", meta = (AllowPrivateAccess="true"))
	TSoftObjectPtr<UAnimMontage> RollMontage;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> Roll;
};

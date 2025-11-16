// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BC_MontageComponent.generated.h"

class UAnimMontage;

UENUM(BlueprintType)
enum class EBC_MontageType : uint8
{
	EMT_QuickAttack UMETA(DisplayName = "Quick Attack"),
	EMT_HitReact UMETA(DisplayName = "Hit React"),
	EMT_EquipWeapon UMETA(DisplayName = "Equip Weapon"),
	EMT_Roll UMETA(DisplayName = "Roll")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASECRAFT_API UBC_MontageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBC_MontageComponent();

private:
	bool bMontagesLoaded;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BaseCraft|Montages|Combat", meta = (AllowPrivateAccess="true"))
	TSoftObjectPtr<UAnimMontage> QuickAttackMontage;

	UPROPERTY()
	TObjectPtr<UAnimMontage> QuickAttack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BaseCraft|Montages|Combat", meta = (AllowPrivateAccess="true"))
	TSoftObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY()
	TObjectPtr<UAnimMontage> HitReact;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BaseCraft|Montages|Combat", meta = (AllowPrivateAccess="true"))
	TSoftObjectPtr<UAnimMontage> EquipWeaponMontage;

	UPROPERTY()
	TObjectPtr<UAnimMontage> EquipWeapon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BaseCraft|Montages|Combat", meta = (AllowPrivateAccess="true"))
	TSoftObjectPtr<UAnimMontage> RollMontage;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> Roll;
	
public:

	UFUNCTION(BlueprintCallable, Category = "BaseCraft|Montages|Combat")
	void LoadMontages();
	
	UFUNCTION(BlueprintCallable, Category = "BaseCraft|Montages|Combat")
	void PlayMontage(EBC_MontageType MontageType, FName SectionName = NAME_None);

	void PlayMontageWithEndDelegate(FOnMontageEnded& EndDelegate, EBC_MontageType MontageType, FName SectionName = NAME_None);

	UFUNCTION(BlueprintPure, Category = "BaseCraft|Montages|Combat")
	FName GetHitReactMontageSectionName(const FVector& ImpactPoint) const;
	
	UFUNCTION(BlueprintCallable, Category = "BaseCraft|Montages")
	void SetMontage(EBC_MontageType MontageType, TSoftObjectPtr<UAnimMontage> NewMontage);
	
	UFUNCTION(BlueprintCallable, Category="BaseCraft|Montages")
	bool IsPlayingMontage(EBC_MontageType MontageType);
};

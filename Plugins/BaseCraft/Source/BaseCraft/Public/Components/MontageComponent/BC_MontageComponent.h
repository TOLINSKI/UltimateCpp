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

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract)
class BASECRAFT_API UBC_MontageComponent : public UActorComponent
{
	GENERATED_BODY()

using enum EBC_MontageType;
	
public:
	UBC_MontageComponent();

protected:
	// virtual void OnRegister() override;	
	virtual void BeginPlay() override;
	
	void SetMontagesLoaded(bool bNewMontagesLoaded) { bMontagesLoaded = bNewMontagesLoaded; }
	
private:
	bool bMontagesLoaded;
	
public:
	UFUNCTION(BlueprintCallable, Category = "BaseCraft|Montages|Combat")
	void LoadMontages();
	
	UFUNCTION(BlueprintCallable, Category = "BaseCraft|Montages|Combat")
	virtual void PlayMontage(EBC_MontageType MontageType, FName SectionName = NAME_None) {}

	virtual void PlayMontageWithEndDelegate(FOnMontageEnded& EndDelegate, EBC_MontageType MontageType, FName SectionName = NAME_None) {}

	UFUNCTION(BlueprintPure, Category = "BaseCraft|Montages|Combat")
	FName GetHitReactMontageSectionName(const FVector& ImpactPoint) const;
	
	UFUNCTION(BlueprintCallable, Category="BaseCraft|Montages")
	virtual bool IsPlayingMontage(EBC_MontageType MontageType) { return false; }
	
	virtual void GetMontagesSoftPaths(TArray<FSoftObjectPath>& SoftObjectPaths) const {}
	
	UFUNCTION()
	virtual void OnMontagesLoaded() { bMontagesLoaded = true; }
	
	UFUNCTION(BlueprintCallable, Category = "BaseCraft|Montages")
	bool IsMontagesLoaded() const { return bMontagesLoaded; }
};



// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/BC_Attacker.h"
#include "Interfaces/BC_Damageable.h"
#include "BC_Character.generated.h"

class UBC_MontageComponent;
class UBC_AttributeComponent;
class UBC_HitStopComponent;
class ABC_Weapon;

UCLASS(Abstract)
class BASECRAFT_API ABC_Character : public ACharacter, public IBC_Attacker, public IBC_Damageable
{
	GENERATED_BODY()

public:
	ABC_Character();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BaseCraft|Components", meta = (AllowPrivateAccess="true"))
	TObjectPtr<UBC_MontageComponent> MontageManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BaseCraft|Components", meta = (AllowPrivateAccess="true"))
	TObjectPtr<UBC_AttributeComponent> Attributes; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BaseCraft|Components", meta = (AllowPrivateAccess="true"))
	TObjectPtr<UBC_HitStopComponent> HitStop; 
	
protected:
	UFUNCTION(BlueprintCallable, Category = "BaseCraft|State")
	virtual void HandleDeath();

	UFUNCTION()
	virtual void OnWeaponHit(const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BaseCraft|Combat", meta = (AllowPrivateAccess = "true"))
	float DeathImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BaseCraft|Combat", meta = (AllowPrivateAccess="true"))
	TWeakObjectPtr<ABC_Weapon> EquippedWeapon;
	
public:
	//~ Begin BC Attacker Interface
	virtual UObject* GetWeapon_Implementation() override;
	virtual void QuickAttack_Implementation() override;
	virtual void EquipWeapon_Implementation(UObject* Weapon) override;
	virtual void UnequipWeapon_Implementation() override;
	//~ End BC Attacker Interface

	//~ Begin BC Damageable Interface
	virtual void TakeDamage_Implementation(AActor* Causer, float Damage, const FHitResult& Hit) override;
	//~ Begin BC Damageable Interface

	FORCEINLINE UBC_MontageComponent* GetMontageManager() const { return MontageManager; }
	FORCEINLINE UBC_AttributeComponent* GetAttributes() const { return Attributes; }
	FORCEINLINE UBC_HitStopComponent* GetHitStopComponent() const { return HitStop; }
};

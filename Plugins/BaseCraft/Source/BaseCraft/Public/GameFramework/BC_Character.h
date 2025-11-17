// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/BC_Attacker.h"
#include "Interfaces/BC_Damageable.h"
#include "Interfaces/BC_Attributes.h"
#include "BC_Character.generated.h"

class UBC_CharacterMontageComponent;
class UBC_AttributeComponent;
class UBC_HitStopComponent;
class ABC_Weapon;
class UBC_MontageComponent;

UCLASS(Abstract)
class BASECRAFT_API ABC_Character : public ACharacter, public IBC_Attacker, public IBC_Damageable, public IBC_Attributes
{
	GENERATED_BODY()

public:
	ABC_Character();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BaseCraft|Components", meta = (AllowPrivateAccess="true"))
	TObjectPtr<UBC_CharacterMontageComponent> MontageManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BaseCraft|Components", meta = (AllowPrivateAccess="true"))
	TObjectPtr<UBC_AttributeComponent> Attributes; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BaseCraft|Components", meta = (AllowPrivateAccess="true"))
	TObjectPtr<UBC_HitStopComponent> HitStop; 
	
protected:
	UFUNCTION(BlueprintCallable, Category = "BaseCraft|State")
	virtual void HandleDeath();

	UFUNCTION()
	virtual void OnWeaponHit(const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Combat", meta = (AllowPrivateAccess = "true"))
	float DeathImpulse;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="BaseCraft|Combat", meta = (AllowPrivateAccess="true"))
	TWeakObjectPtr<ABC_Weapon> EquippedWeapon;
	
	AActor* GetNearestInteractable() const;	
	
public:
	//~ Begin BC Attacker Interface
	virtual UObject* GetWeapon_Implementation() override;
	virtual void EquipWeapon_Implementation(UObject* Weapon) override;
	virtual void UnequipWeapon_Implementation() override;
	//~ End BC Attacker Interface

	//~ Begin BC Damageable Interface
	virtual void TakeDamage_Implementation(AActor* Causer, float Damage, const FHitResult& Hit) override;
	//~ Begin BC Damageable Interface
	
	//~Begin Attributes Interface
	virtual float GetHealth() const override;
	virtual bool IsAlive() const override;
	virtual float GetHealthPercent() const override;
	//~End Attributes Interface
	
	UFUNCTION(BlueprintCallable, Category = "BaseCraft|Combat")
	UBC_MontageComponent* GetWeaponMontageManager() const;
	
	UFUNCTION(BlueprintCallable, Category = "BaseCraft|Combat")
	virtual void PlayQuickAttackMontage();
	
	UFUNCTION(BlueprintCallable, Category = "BaseCraft|Combat")
	virtual void PlayEquipMontage();
	
	UFUNCTION(BlueprintCallable, Category = "BaseCraft|Combat")
	virtual void PlayUnequipMontage();
	
	FORCEINLINE UBC_MontageComponent* GetMontageManager() const;
	FORCEINLINE UBC_AttributeComponent* GetAttributes() const { return Attributes; }
	FORCEINLINE UBC_HitStopComponent* GetHitStopComponent() const { return HitStop; }
};

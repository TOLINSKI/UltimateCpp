// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/BC_PlayerCharacter.h"
#include "Combat/SlashWeapon.h"
#include "Utility/SlashTypes.h"
#include "SlashCharacter.generated.h"

class UInputMappingContext;
struct FInputActionValue;
class ABC_Item;
class UAnimMontage;
class ASlashWeapon;

UCLASS()
class ULTIMATECPP_API ASlashCharacter : public ABC_PlayerCharacter
{
	GENERATED_BODY()

// UE interface:
public:
	ASlashCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

// Slash interface:
public:
	//~ Begin Attacker Interface
	virtual void SetComboWindowActive_Implementation(bool bComboWindowActive) override;
	virtual void SetAttackBufferWindowActive_Implementation(bool bAttackBufferWindowActive) override;
	virtual void EquipWeapon_Implementation(UObject* Weapon) override;
	virtual void UnequipWeapon_Implementation() override;
	virtual void SetInvulnerable_Implementation(bool bInvulnerable) override;
	//~ End Attacker Interface

	//~ Begin Damageable Interface
	virtual void TakeDamage_Implementation(AActor* Causer, float Damage, const FHitResult& Hit) override;
	//~ End Damageable Interface
	
private:

	bool bIsComboWindowActive { false };
	bool bIsAttackBufferWindowActive { false };
	int32 ComboCount { 0 };
	bool bAttackBuffer { false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBC_MontageComponent> ShortSwordMontages; 

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Slash|State", meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState { ECharacterState::ECS_Unequipped };

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Slash|State", meta = (AllowPrivateAccess = "true"))
	EActionState ActionState { EActionState::EAS_Unoccupied };
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Slash|State", meta = (AllowPrivateAccess = "true"))
	EAttackQuickState AttackQuickState { EAttackQuickState::EQS_None };
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Slash|State", meta = (AllowPrivateAccess = "true"))
	ESlashCombatState CombatState { ESlashCombatState::ESC_None };

	UPROPERTY()
	ASlashWeapon* LastUsedWeapon;
	
	void QuickAttackCombo();

	void PlayQuickAttackMontage();

	UBC_MontageComponent* GetCorrectMontageComp();
	
protected:
	//~ Begin input
	virtual void DoMove(const float RightVal, const float ForwardVal) override;
	virtual void DoInteract() override;
	virtual void DoQuickAttack() override;
	virtual void DoRoll() override;
	//~ End input

	//~ Begin Weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slash|Weapon")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slash|Weapon")
	FName BackSocketName;

	virtual void OnWeaponHit(const FHitResult& Hit) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Slash|Weapon")
	void OnWeaponHit_BP(const FHitResult& Hit);
	//~ End Weapons
	
public:
	UFUNCTION(BlueprintPure, Category = "Slash|Weapon")
	FName GetBackSocketName() const { return BackSocketName; }
	//~ End Weapon
	
	UFUNCTION(BlueprintPure, Category = "Slash|Action State")
	bool CanAttack() const;

	UFUNCTION(BlueprintPure, Category = "Slash|Action State")
	FORCEINLINE bool CanMove() const { return ActionState == EActionState::EAS_Unoccupied; }

	UFUNCTION(BlueprintPure, Category = "Slash|Character State")
	FORCEINLINE bool IsEquipped() const { return CharacterState == ECharacterState::ECS_Equipped; }

	UFUNCTION(BlueprintPure, Category = "Slash|Interaction")
	FORCEINLINE bool IsOccupied() const { return ActionState != EActionState::EAS_Unoccupied; }

	UFUNCTION(BlueprintPure, Category = "Slash|Weapon")
	FORCEINLINE ASlashWeapon* GetSlashWeapon();
	
	UFUNCTION(BlueprintCallable, Category = "Slash|Character State")
	FORCEINLINE void SetCharacterState(ECharacterState NewCharacterState) { CharacterState = NewCharacterState; }

	UFUNCTION(BlueprintPure, Category = "Slash|CharacterState")
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	UFUNCTION(BlueprintPure, Category = "Slash|Weapon")
	ESlashWeaponType GetEquippedWeaponType();
	
	UFUNCTION(BlueprintPure, Category = "Slash|Weapon")
	ESlashCombatState GetCombatState() const { return CombatState; }
};

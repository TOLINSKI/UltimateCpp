// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utility/SlashTypes.h"
#include "SlashCharacter.generated.h"

class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
struct FInputActionValue;
class ABC_Item;
class UAnimMontage;
class ASlashWeapon;

UCLASS()
class ULTIMATECPP_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

// UE interface:
public:
	ASlashCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

// Slash interface:
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slash|Camera", meta = (AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slash|Camera", meta = (AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> Camera;
	
	//~ Begin input 
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	virtual void StopJumping() override;
	void AttackQuickCombo(); 
	//~ End input

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Slash|Interaction", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AActor> Interactable;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Slash|State", meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState { ECharacterState::ECS_Unequipped };

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Slash|State", meta = (AllowPrivateAccess = "true"))
	EActionState ActionState { EActionState::EAS_Unoccupied };
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Slash|State", meta = (AllowPrivateAccess = "true"))
	EAttackQuickState AttackQuickState { EAttackQuickState::EQS_None };

	int32 ComboCount { 0 };
	bool bAttackQuickCombo { false };
	bool bAttackBufferFull { false };
	
protected:
	
	//~ Begin input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Input")
	TObjectPtr<UInputMappingContext> MovementInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Input")
	TObjectPtr<UInputAction> MouseLookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Input")
	TObjectPtr<UInputAction> InteractAction; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Input")
	TObjectPtr<UInputAction> AttackQuickAction; 
	
	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	void DoMove(const float Right, const float Forward);

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	void DoLook(const float Yaw, const float Pitch);

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	void DoJump();

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	void DoStopJumping();

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	void DoInteract();

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	void DoAttackQuick();
	//~ End input

	//~ Begin Animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Animation")
	TObjectPtr<UAnimMontage> AttackQuickMontage; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slash|Animation")
	TObjectPtr<UAnimMontage> EquipWeaponMontage; 

	/** Begins playing montage and returns all relevant skeletal meshes. */
	UFUNCTION(BlueprintCallable, Category = "Slash|Animation")
	TArray<USkeletalMeshComponent*> PlayMontage_SkeletalMeshHierarchy(UAnimMontage* Montage, const FName& SectionName);

	/** Begins playing montage and returns all relevant skeletal meshes. */
	void BindOnMontageEndedDelegate_SkeletalMeshHierarchy(UAnimMontage* Montage, FOnMontageEnded& Delegate);
	//~ End Animation

	//~ Begin Weapons
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ASlashWeapon> EquippedWeapon;
	
	//~ End Weapons
	
public:
	UFUNCTION(BlueprintPure, Category = "Slash|Action State")
	bool CanAttack() const;

	UFUNCTION(BlueprintPure, Category = "Slash|Action State")
	FORCEINLINE bool CanMove() const { return ActionState == EActionState::EAS_Unoccupied; }

	UFUNCTION(BlueprintPure, Category = "Slash|Character State")
	FORCEINLINE bool IsEquipped() const { return CharacterState == ECharacterState::ECS_Equipped; }

	UFUNCTION(BlueprintPure, Category = "Slash|Interaction")
	FORCEINLINE bool IsOccupied() const { return ActionState != EActionState::EAS_Unoccupied; }

	UFUNCTION(BlueprintCallable, Category = "Slash|Interaction")
	void SetInteractable(AActor* Item);
	
	UFUNCTION(BlueprintPure, Category = "Slash|Interaction")
	FORCEINLINE AActor* GetInteractable() const { return Interactable.Get(); }

	UFUNCTION(BlueprintCallable, Category = "Slash|Weapon")
	void SetWeapon(ASlashWeapon* Weapon);

	UFUNCTION(BlueprintPure, Category = "Slash|Weapon")
	FORCEINLINE ASlashWeapon* GetWeapon() const { return EquippedWeapon.Get(); }
	
	UFUNCTION(BlueprintCallable, Category = "Slash|Character State")
	FORCEINLINE void SetCharacterState(ECharacterState NewCharacterState) { CharacterState = NewCharacterState; }

	UFUNCTION(BlueprintPure, Category = "Slash|CharacterState")
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	FORCEINLINE void SetAttackQuickCombo(bool bCombo) { bAttackQuickCombo = bCombo;}
};

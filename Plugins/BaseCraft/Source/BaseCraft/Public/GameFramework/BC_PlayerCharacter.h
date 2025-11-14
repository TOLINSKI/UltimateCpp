// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/BC_Character.h"
#include "BC_PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UBC_AttributeComponent;


UCLASS(Abstract)
class BASECRAFT_API ABC_PlayerCharacter : public ABC_Character
{
	GENERATED_BODY()

public:
	ABC_PlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
// Base Craft:
// ===========

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slash|Camera", meta = (AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slash|Camera", meta = (AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> Camera;

	//~ Begin input 
	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void Interact();
	virtual void QuickAttack_Implementation() override;
	//~ End Input

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Slash|Interaction", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AActor> Interactable;

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
	TObjectPtr<UInputAction> QuickAttackAction; 
	
	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	virtual void DoMove(const float Right, const float Forward);

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	virtual void DoLook(const float Yaw, const float Pitch);

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	virtual void DoJump();

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	virtual void DoStopJumping();

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	virtual void DoInteract();

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	virtual void DoQuickAttack();
	
public:
	AActor* GetInteractable() const { return Interactable.Get(); }
	void SetInteractable(AActor* NewInteractable);

	FORCEINLINE UCameraComponent* GetCamera() const { return Camera;}
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom;}
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return CameraBoom;}
};

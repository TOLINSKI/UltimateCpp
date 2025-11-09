// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/BC_AttackerInterface.h"
#include "Interfaces/BC_DamageableInterface.h"
#include "BC_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


UCLASS()
class BASECRAFT_API ABC_Character : public ACharacter, public IBC_AttackerInterface, public IBC_DamageableInterface
{
	GENERATED_BODY()

public:
	ABC_Character();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
// Base Craft:
// ===========
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
	//~ End Input
	
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
	virtual void DoMove(const float Right, const float Forward);

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	virtual void DoLook(const float Yaw, const float Pitch);

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	virtual void DoJump();

	UFUNCTION(BlueprintCallable, Category = "Slash|Input")
	virtual void DoStopJumping();
};

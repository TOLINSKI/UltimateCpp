// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Utility/BC_Types.h"
#include "BC_PlayerAnimInstance.generated.h"

class UCharacterMovementComponent;

UCLASS()
class BASECRAFT_API UBC_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	TObjectPtr<ACharacter> Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner|Movement")
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner|Movement")
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner|Movement")
	float Direction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner|Movement")
	bool bHasGroundVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner|Movement")
	bool bIsFalling;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner|State")
	EBC_WeaponType EquippedWeaponType;
};

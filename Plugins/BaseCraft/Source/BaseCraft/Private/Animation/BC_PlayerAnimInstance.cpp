// Copyright Benski Game Works 2025, All rights reserved.


#include "Animation/BC_PlayerAnimInstance.h"
#include "kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/BC_Attacker.h"
#include "Interfaces/BC_WeaponInterface.h"

void UBC_PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (Character = Cast<ACharacter>(TryGetPawnOwner()); Character != nullptr)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UBC_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!(Character && MovementComponent))
		return;
	
	Velocity = MovementComponent->Velocity;
	GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
	Direction = UKismetAnimationLibrary::CalculateDirection(MovementComponent->Velocity, Character->GetActorRotation());
	if (MovementComponent->bOrientRotationToMovement)
	{
		Direction = FMath::Clamp(Direction, -45.0f, 45.0f);
	}
	bHasGroundVelocity = GroundSpeed > 0.01f && !MovementComponent->GetCurrentAcceleration().IsNearlyZero();
	bIsFalling = MovementComponent->IsFalling();

	if (Character->Implements<UBC_Attacker>())
	{
		if (const UObject* Weapon = IBC_Attacker::Execute_GetWeapon(Character); Weapon && Weapon->Implements<UBC_WeaponInterface>())
		{
			EquippedWeaponType = static_cast<EBC_WeaponType>(IBC_WeaponInterface::Execute_GetWeaponType(Weapon));
		}
	}
}

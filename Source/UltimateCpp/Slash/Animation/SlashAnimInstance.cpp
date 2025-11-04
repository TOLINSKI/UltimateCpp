// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Slash/SlashCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogSlashAnimInstance, All, All);

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(TryGetPawnOwner());
	if (!Character)
		return;

	MovementComponent = Character->GetCharacterMovement();

	SlashCharacter = Cast<ASlashCharacter>(Character);
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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

	if (!SlashCharacter)
		return;

	CharacterState = SlashCharacter->GetCharacterState();
}




// Copyright Benski Game Works 2025, All rights reserved.


#include "GameFramework//BC_PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/BC_Interactable.h"

DEFINE_LOG_CATEGORY_STATIC(Log_BC_PlayerCharacter, All, All);

ABC_PlayerCharacter::ABC_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Comp"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	Camera->SetupAttachment(CameraBoom);
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;	
}
void ABC_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	check(PlayerController != nullptr);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	check(Subsystem != nullptr);

	if (!ensureMsgf(MovementInputMappingContext != nullptr, TEXT("Movement input mapping context invalid!")))
		return;
	
	Subsystem->AddMappingContext(MovementInputMappingContext, 0);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

		// Looking
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);

		// Attacking
		EnhancedInputComponent->BindAction(QuickAttackAction, ETriggerEvent::Started, this, &ThisClass::QuickAttack);
		
		// Rolling
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &ThisClass::Roll);
	}
	else
	{
		UE_LOG(Log_BC_PlayerCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

// Base Craft
// ===========

//~ Begin Input
void ABC_PlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}
void ABC_PlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	float Yaw = LookVector.X;
	float Pitch = LookVector.Y;
	DoLook(Yaw, Pitch);
}
void ABC_PlayerCharacter::Jump()
{
	DoJump();
}
void ABC_PlayerCharacter::StopJumping()
{
	DoStopJumping();
}
void ABC_PlayerCharacter::Interact()
{
	DoInteract();
}
void ABC_PlayerCharacter::QuickAttack()
{
	DoQuickAttack();
}
void ABC_PlayerCharacter::Roll()
{
	DoRoll();
}

//~ End Input
void ABC_PlayerCharacter::DoMove(const float Right, const float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}
void ABC_PlayerCharacter::DoLook(const float Yaw, const float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}
void ABC_PlayerCharacter::DoJump()
{
	Super::Jump();
}
void ABC_PlayerCharacter::DoStopJumping()
{
	Super::StopJumping();
}
void ABC_PlayerCharacter::DoInteract_Implementation()
{
	if (AActor* Interactable = GetNearestInteractable())
	{
		IBC_Interactable::Execute_Interact(Interactable, this);
	}
}
void ABC_PlayerCharacter::DoQuickAttack_Implementation()
{
	UE_LOG(Log_BC_PlayerCharacter, Display, TEXT("Character: %s Quick Attack!"), *GetName());
}
void ABC_PlayerCharacter::DoRoll_Implementation()
{
	UE_LOG(Log_BC_PlayerCharacter, Display, TEXT("Character: %s Rolls!"), *GetName());
}

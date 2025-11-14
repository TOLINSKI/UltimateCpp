// Copyright Benski Game Works 2025, All rights reserved.


#include "GameFramework//BC_PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BC_MontageComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/BC_Interactable.h"

DEFINE_LOG_CATEGORY_STATIC(Log_BC_Character, All, All);

ABC_PlayerCharacter::ABC_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Comp"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	Camera->SetupAttachment(CameraBoom);
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
		EnhancedInputComponent->BindAction(QuickAttackAction, ETriggerEvent::Started, this, &ThisClass::QuickAttack_Implementation);
	}
	else
	{
		UE_LOG(Log_BC_Character, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
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
void ABC_PlayerCharacter::QuickAttack_Implementation()
{
	Super::QuickAttack_Implementation();
	
	DoQuickAttack();
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
void ABC_PlayerCharacter::DoInteract()
{
	if (AActor* Item = GetInteractable())
	{
		IBC_Interactable::Execute_Interact(Item, this);
	}
}
void ABC_PlayerCharacter::DoQuickAttack()
{
	GetMontageManager()->PlayMontage(EBC_MontageType::EMT_QuickAttack);
}
void ABC_PlayerCharacter::SetInteractable(AActor* NewInteractable)
{
	// If Item is valid but doesn't implement IBC_Interactable interface >> Do nothing
	if (NewInteractable && !ensureMsgf(NewInteractable->Implements<UBC_Interactable>(), TEXT("Attempt to set Item that does not implement IBC_Interactable interface.")))
		return;

	Interactable = NewInteractable;
}

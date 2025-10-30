// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Items/Weapons/SlashWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogSlashCharacter, All, All);

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	float Yaw = LookVector.X;
	float Pitch = LookVector.Y;
	DoLook(Yaw, Pitch);
}

void ASlashCharacter::Jump()
{
	DoJump();
}

void ASlashCharacter::StopJumping()
{
	DoStopJumping();
}

void ASlashCharacter::DoMove(const float Right, const float Forward)
{
	if (!CanMove())
		return;
	
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

void ASlashCharacter::DoLook(const float Yaw, const float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ASlashCharacter::DoJump()
{
	if (!CanMove())
		return;
	
	Super::Jump();
}

void ASlashCharacter::DoStopJumping()
{
	Super::StopJumping();
}

void ASlashCharacter::DoInteract()
{
	if (IsOccupied())
		return;
	
	if (AActor* Item = Interactable.Get())
	{
		IBC_Interactable::Execute_Interact(Item, this);
		return;
	}
	
	if (IsEquipped())
	{
		// Play Montage
		PlayMontage_SkeletalMeshHierarchy(EquipWeaponMontage, TEXT("Unequip"));
		ActionState = EActionState::EAS_Equipping;
		CharacterState = ECharacterState::ECS_Unequipped;
		
		// Bind On Montage Ended Delegate
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindLambda(
			[&](UAnimMontage* AnimMontage, bool bInterrupted)
			{
				ActionState = EActionState::EAS_Unoccupied;
			});
		BindOnMontageEndedDelegate_SkeletalMeshHierarchy(EquipWeaponMontage, MontageEndedDelegate);
	}
	else if (EquippedWeapon.IsValid())
	{
		PlayMontage_SkeletalMeshHierarchy(EquipWeaponMontage, TEXT("Equip"));
		EquippedWeapon.Get()->AttachToHand(this);
		
		ActionState = EActionState::EAS_Equipping;
		CharacterState = ECharacterState::ECS_Equipped;
		
		// Bind On Montage Ended Delegate
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindLambda(
			[&](UAnimMontage* AnimMontage, bool bInterrupted)
			{
				ActionState = EActionState::EAS_Unoccupied;
			});
		BindOnMontageEndedDelegate_SkeletalMeshHierarchy(EquipWeaponMontage, MontageEndedDelegate);
	}
}

void ASlashCharacter::DoAttackQuick()
{
	if (!CanAttack())
		return;

	AttackQuickCombo();
}

void ASlashCharacter::AttackQuickCombo()
{
	if (AttackQuickMontage)
	{
		if (bAttackQuickCombo
			&& !bAttackBufferFull
			&& ComboCount < 2)
		{
			bAttackBufferFull = true;
			ComboCount++;
			return;
		}
		
		// Play Montage
		FName SectionName = FName(FString::Printf(TEXT("Attack%d"), ComboCount + 1));
		TArray<USkeletalMeshComponent*> SkeletalMeshes = PlayMontage_SkeletalMeshHierarchy(AttackQuickMontage, SectionName);
		ActionState = EActionState::EAS_Attacking;
				
		// Bind On End Anim Montage Delegate
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindLambda(
			[&](UAnimMontage* AnimMontage, bool bInterrupted)
			{
				bAttackQuickCombo = false;
				
				if (bAttackBufferFull)
				{
					bAttackBufferFull = false;
					AttackQuickCombo();
					return;
				}
				
				bAttackBufferFull = false;
				ComboCount = 0;
				ActionState = EActionState::EAS_Unoccupied;
			});
		BindOnMontageEndedDelegate_SkeletalMeshHierarchy(AttackQuickMontage, MontageEndedDelegate);
	}
}

TArray<USkeletalMeshComponent*> ASlashCharacter::PlayMontage_SkeletalMeshHierarchy(UAnimMontage* Montage, const FName& SectionName)
{
	if (!ensureMsgf(Montage != nullptr, TEXT("Invalid montage")))
		return TArray<USkeletalMeshComponent*>();
	
	// Array of relevant skeletal meshes to return
	TArray<USkeletalMeshComponent*> Result;

	// Temp array
	TArray<USceneComponent*> MeshComponents;
	GetMesh()->GetChildrenComponents(false, MeshComponents);
	MeshComponents.Add(GetMesh());

	for (USceneComponent* MeshComponent : MeshComponents)
	{
		if (USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(MeshComponent))
		{
			if (UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance())
			{
				Result.Add(SkeletalMesh);
				AnimInstance->Montage_Play(Montage);
				if (!SectionName.IsNone())
					AnimInstance->Montage_JumpToSection(SectionName);
			}
		}
	}
	return Result;
}

void ASlashCharacter::BindOnMontageEndedDelegate_SkeletalMeshHierarchy(UAnimMontage* Montage,
	FOnMontageEnded& Delegate)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_SetEndDelegate(Delegate);
	}
}

bool ASlashCharacter::CanAttack() const
{
	return  IsEquipped()
			&& (ActionState == EActionState::EAS_Attacking ? bAttackQuickCombo : true)
			&& !bAttackBufferFull
			&& ComboCount < 3
			&& GetMovementComponent()
			&& !GetMovementComponent()->IsFalling();
}

void ASlashCharacter::SetInteractable(AActor* Item)
{
	// If Item is valid but doesn't implement IBC_Interactable interface >> Do nothing
	if (Item && !ensureMsgf(Item->Implements<UBC_Interactable>(), TEXT("Attempt to set Item that does not implement IBC_Interactable interface.")))
		return;

	// Otherwise, set Interactable as Item
	Interactable = Item;
}

void ASlashCharacter::SetWeapon(ASlashWeapon* Weapon)
{
	EquippedWeapon = Weapon;
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::DoInteract);

		// Attacking
		EnhancedInputComponent->BindAction(AttackQuickAction, ETriggerEvent::Started, this, &ThisClass::DoAttackQuick);
	}
	else
	{
		UE_LOG(LogSlashCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}


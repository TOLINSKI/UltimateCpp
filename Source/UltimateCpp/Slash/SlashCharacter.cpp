// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashCharacter.h"
#include "Animation/AnimMontage.h"
#include "Combat/SlashWeapon.h"
#include "Components/BC_MontageComponent.h"
#include "Interfaces/BC_Interactable.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSlashCharacter, All, All);

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ShortSwordMontages = CreateDefaultSubobject<UBC_MontageComponent>(TEXT("Montage Comp For Short Sword"));
}
void ASlashCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Temp array
	TArray<USceneComponent*> MeshComponents;
	GetMesh()->GetChildrenComponents(false, MeshComponents);
	
	for (USceneComponent* MeshComponent : MeshComponents)
	{
		if (USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(MeshComponent))
		{
			SkeletalMesh->SetLeaderPoseComponent(GetMesh());
		}
	}
}
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	ShortSwordMontages->LoadMontages();
}
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//~ Begin BC Attacker Interface
void ASlashCharacter::SetComboWindowActive_Implementation(bool bComboWindowActive)
{
	bIsComboWindowActive = bComboWindowActive;

	// When combo window is opened, attack if buffering
	if (bIsComboWindowActive && bAttackBuffer)
	{
		bAttackBuffer = false;
		QuickAttackCombo();
		bIsComboWindowActive = false;
	}
}
void ASlashCharacter::SetAttackBufferWindowActive_Implementation(bool bAttackBufferWindowActive)
{
	bIsAttackBufferWindowActive = bAttackBufferWindowActive;
}
void ASlashCharacter::EquipWeapon_Implementation(UObject* NewWeapon)
{
	Super::EquipWeapon_Implementation(NewWeapon);
	
	ASlashWeapon* Weapon = GetSlashWeapon();
	if (!ensureMsgf(Weapon, TEXT("EquipWeapon called but no valid weapon found.")))
		return;

	CharacterState = ECharacterState::ECS_Equipped;
	ActionState = EActionState::EAS_Equipping;
		
	// Bind On Montage Ended Delegate
	FOnMontageEnded EndDelegate;
	EndDelegate.BindWeakLambda(this,
		[this](UAnimMontage* AnimMontage, bool bInterrupted)
		{
			ActionState = EActionState::EAS_Unoccupied;
			CombatState = ESlashCombatState::ESC_Normal;
		});

	GetMontageManager()->PlayMontageWithEndDelegate(EndDelegate, EBC_MontageType::EMT_EquipWeapon, TEXT("Equip"));
	IBC_WeaponInterface::Execute_Attach(Weapon, GetMesh(), HandSocketName);
}
void ASlashCharacter::UnequipWeapon_Implementation()
{
	// Bind On Montage Ended Delegate
	FOnMontageEnded EndDelegate;
	EndDelegate.BindWeakLambda(this,
		[this](UAnimMontage* AnimMontage, bool bInterrupted)
		{
			ActionState = EActionState::EAS_Unoccupied;
			CombatState = ESlashCombatState::ESC_None;
			LastUsedWeapon = GetSlashWeapon();
			Super::UnequipWeapon_Implementation();
		});

	// Play Montage
	GetMontageManager()->PlayMontageWithEndDelegate(EndDelegate, EBC_MontageType::EMT_EquipWeapon, TEXT("Unequip"));
	ActionState = EActionState::EAS_Equipping;
	CharacterState = ECharacterState::ECS_Unequipped;
}

void ASlashCharacter::SetInvulnerable_Implementation(bool bInvaulnerable)
{
	if (bInvaulnerable)
		CombatState = ESlashCombatState::ESC_Invulnerable;
	else
		CombatState = ESlashCombatState::ESC_Normal;	
}

void ASlashCharacter::TakeDamage_Implementation(AActor* Causer, float Damage, const FHitResult& Hit)
{
	// Ensure vulnerable
	if (CombatState == ESlashCombatState::ESC_Invulnerable)
		return;
	
	Super::TakeDamage_Implementation(Causer, Damage, Hit);

	// When doing last combo do not stagger
	if (ActionState == EActionState::EAS_Attacking && ComboCount == 3)
		return;

	if (ActionState == EActionState::EAS_Attacking)
		IBC_WeaponInterface::Execute_EndAttackTracing(GetSlashWeapon());
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindWeakLambda(this,
		[this](UAnimMontage* AnimMontage, bool bInterrupted)
		{
			bAttackBuffer = false;
			ComboCount = 0;
			ActionState = EActionState::EAS_Unoccupied;
		});
	
	FName SectionName = GetMontageManager()->GetHitReactMontageSectionName(Causer->GetActorLocation());
	GetMontageManager()->PlayMontageWithEndDelegate(EndDelegate, EBC_MontageType::EMT_HitReact, SectionName);
}
//~ End BC Attacker Interface

//~ Begin input
void ASlashCharacter::DoMove(const float RightVal, const float ForwardVal)
{
	if (!CanMove())
		return;
	
	Super::DoMove(RightVal, ForwardVal);
}
void ASlashCharacter::DoInteract()
{
	if (IsOccupied())
		return;
	
	if (AActor* Item = GetNearestInteractable())
	{
		IBC_Interactable::Execute_Interact(Item, this);
		return;
	}
	
	if (IsEquipped())
	{
		Execute_UnequipWeapon(this);
	}
	else if (LastUsedWeapon)
	{
		Execute_EquipWeapon(this, LastUsedWeapon);
	}
}
void ASlashCharacter::DoQuickAttack()
{
	if (!CanAttack())
		return;

	QuickAttackCombo();
}

void ASlashCharacter::DoRoll()
{
	if (!CanAttack())
		return;
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindWeakLambda(this,
		[this](UAnimMontage* AnimMontage, bool bInterrupted)
		{
			CombatState = ESlashCombatState::ESC_Normal;
			
			if (!bInterrupted)
				ActionState = EActionState::EAS_Unoccupied;
		});
	
	GetMontageManager()->PlayMontageWithEndDelegate(EndDelegate, EBC_MontageType::EMT_Roll);
}

//~ End input

//~ Begin Weapon
void ASlashCharacter::OnWeaponHit(const FHitResult& Hit)
{
	Super::OnWeaponHit(Hit);
	
	OnWeaponHit_BP(Hit);
}
void ASlashCharacter::QuickAttackCombo()
{
	if (bIsAttackBufferWindowActive && !bIsComboWindowActive)
	{
		bAttackBuffer = true;
		return;
	}
	
	ComboCount++;
	ActionState = EActionState::EAS_Attacking;
	
	PlayQuickAttackMontage();
}
void ASlashCharacter::PlayQuickAttackMontage()
{
	// Make On End Anim Montage Delegate
	FOnMontageEnded EndDelegate;
	EndDelegate.BindWeakLambda(this,
		[this](UAnimMontage* AnimMontage, bool bInterrupted)
		{
			if (bInterrupted)
			{
				return;
			}
			
			bAttackBuffer = false;
			ComboCount = 0;
			ActionState = EActionState::EAS_Unoccupied;
		});

	FName SectionName = FName(FString::Printf(TEXT("Attack%d"), ComboCount));

	if (UBC_MontageComponent* MontageComp = GetCorrectMontageComp())
	{
		MontageComp->PlayMontageWithEndDelegate(EndDelegate, EBC_MontageType::EMT_QuickAttack, SectionName);
	}
}
UBC_MontageComponent* ASlashCharacter::GetCorrectMontageComp()
{
	if (ASlashWeapon* SlashWeapon = GetSlashWeapon())
	{
		ESlashWeaponType WeaponType = SlashWeapon->GetWeaponType();

		switch (WeaponType)
		{
		case ESlashWeaponType::EWT_ShortSword:
			return ShortSwordMontages;
		default:
			return GetMontageManager();
		};
	}
	
	return nullptr;
}
//~ End Weapon

bool ASlashCharacter::CanAttack() const
{
	return  IsEquipped()
			&& ActionState != EActionState::EAS_Equipping
			&& (ActionState == EActionState::EAS_Attacking ? bIsComboWindowActive || bIsAttackBufferWindowActive : true)
			&& !bAttackBuffer
			&& ComboCount < 3
			&& !GetCharacterMovement()->IsFalling();
}
ASlashWeapon* ASlashCharacter::GetSlashWeapon()
{
	return Cast<ASlashWeapon>(Execute_GetWeapon(this));
}

ESlashWeaponType ASlashCharacter::GetEquippedWeaponType()
{
	return GetSlashWeapon() ? GetSlashWeapon()->GetWeaponType() : ESlashWeaponType::EWT_None;
}

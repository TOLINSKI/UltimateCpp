// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashWeapon.h"
#include "SlashCharacter.h"
#include "Components/BC_InteractButtonComponent.h"
#include "Components/BC_HoverComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "kismet/GameplayStatics.h"

ASlashWeapon::ASlashWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASlashWeapon::Equip(ASlashCharacter* Character)
{
	ApplyEquippedSettings();
	AttachToHand(Character);
	Character->SetCharacterState(ECharacterState::ECS_Equipped);
	Character->SetWeapon(this);
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
}

void ASlashWeapon::AttachToHand(ACharacter* Character)
{
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, HandSocketName);
}

void ASlashWeapon::AttachToBack(ACharacter* Character)
{
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, BackSocketName);
}

void ASlashWeapon::ApplyEquippedSettings()
{
	Mesh->SetRelativeTransform(FTransform::Identity);
	
	BC_Hover->SetShouldHover(false);
	BC_Hover->SetComponentTickEnabled(false);
	
	RotatingMovement->RotationRate = FRotator(0, 0, 0);
	RotatingMovement->SetComponentTickEnabled(false);

	PickupSphere->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	bShowPickupSphereInEditor = false;
	PickupSphere->SetHiddenInGame(true);
}

void ASlashWeapon::Interact_Implementation(AActor* InstigatorActor)
{
	if (Mesh->GetStaticMesh() == nullptr)
		return;

	if (ASlashCharacter* Character = Cast<ASlashCharacter>(InstigatorActor))
	{
		bCanInteract = false;
		BC_ButtonWidget->HideButton();
		Character->SetInteractable(nullptr);
		Equip(Character);
	}
}



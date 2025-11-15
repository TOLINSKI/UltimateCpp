// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashItem_Weapon.h"
#include "Components/BC_InteractButtonComponent.h"
#include "kismet/GameplayStatics.h"
#include "Combat/SlashWeapon.h"
#include "Combat/Melee/BC_CapsuleWeapon.h"
#include "Interfaces/BC_Attacker.h"

DEFINE_LOG_CATEGORY_STATIC(LogSlashItemWeapon, All, All);

ASlashItem_Weapon::ASlashItem_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASlashItem_Weapon::Equip(APawn* WeaponOwner)
{
	ASlashWeapon* NewWeapon = Cast<ASlashWeapon>(ABC_Weapon::CreateWeapon(WeaponOwner, WeaponClass));
	check(NewWeapon != nullptr);

	IBC_Attacker::Execute_EquipWeapon(WeaponOwner, NewWeapon);
	
	if (InteractionSound)
		UGameplayStatics::PlaySoundAtLocation(this, InteractionSound, GetActorLocation());
}

void ASlashItem_Weapon::Interact_Implementation(AActor* InstigatorActor)
{
	if (Mesh->GetStaticMesh() == nullptr)
		return;

	if (!WeaponClass)
	{
		UE_LOG(LogSlashItemWeapon, Warning, TEXT("Slash Item Weapon: %s does not have a valid weapon class."), *GetName());
		return;
	}

	if (APawn* Pawn = Cast<APawn>(InstigatorActor))
	{
		bCanInteract = false;
		InteractWidget->HideButton();
		Equip(Pawn);
		Destroy();
	}
}



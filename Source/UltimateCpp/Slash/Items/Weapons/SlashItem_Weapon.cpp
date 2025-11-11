// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashItem_Weapon.h"
#include "SlashCharacter.h"
#include "Components/BC_InteractButtonComponent.h"
#include "kismet/GameplayStatics.h"
#include "Combat/SlashWeapon.h"
#include "Combat/Melee/BC_CapsuleWeapon.h"


ASlashItem_Weapon::ASlashItem_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASlashItem_Weapon::Equip(ASlashCharacter* Character)
{
	ASlashWeapon* NewWeapon = Cast<ASlashWeapon>(ABC_MeleeWeapon::CreateWeapon(Character, WeaponClass));
	check(NewWeapon != nullptr);

	NewWeapon->SetInstigator(Character);
	Character->EquipWeapon(NewWeapon);

	if (InteractionSound)
		UGameplayStatics::PlaySoundAtLocation(this, InteractionSound, GetActorLocation());
}

void ASlashItem_Weapon::Interact_Implementation(AActor* InstigatorActor)
{
	if (Mesh->GetStaticMesh() == nullptr)
		return;

	if (ASlashCharacter* Character = Cast<ASlashCharacter>(InstigatorActor))
	{
		bCanInteract = false;
		BC_ButtonWidget->HideButton();
		Character->SetInteractable(nullptr);
		Equip(Character);
		Destroy();
	}
}



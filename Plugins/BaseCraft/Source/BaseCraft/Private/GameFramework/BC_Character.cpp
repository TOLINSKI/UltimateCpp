// Copyright Benski Game Works 2025, All rights reserved.


#include "GameFramework/BC_Character.h"

#include "Components/BC_AttributeComponent.h"
#include "Components/BC_MontageComponent.h"
#include "Components/BC_HitStopComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Combat/BC_Weapon.h"

DEFINE_LOG_CATEGORY_STATIC(Log_BC_Character, All, All);

ABC_Character::ABC_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	MontageManager = CreateDefaultSubobject<UBC_MontageComponent>(TEXT("Montage Comp"));
	Attributes = CreateDefaultSubobject<UBC_AttributeComponent>(TEXT("Attribute Comp"));
	HitStop = CreateDefaultSubobject<UBC_HitStopComponent>(TEXT("Hit Stop Comp"));

	DeathImpulse = 100000.0f;
}

void ABC_Character::BeginPlay()
{
	Super::BeginPlay();

	GetMontageManager()->LoadMontages();
}

void ABC_Character::HandleDeath()
{
	// enable full ragdoll physics
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);

	// disable the collision capsule to avoid being hit again while dead
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// disable character movement
	GetCharacterMovement()->DisableMovement();
}

void ABC_Character::OnWeaponHit(const FHitResult& Hit)
{
	GetHitStopComponent()->HitStop();
	
	if (AActor* HitActor = Hit.GetActor(); HitActor->Implements<UBC_DamageableInterface>())
	{
		Execute_TakeDamage(HitActor, EquippedWeapon.Get()->GetBaseDamage(), Hit);
	}
}

//~ Begin BC Attacker Interface
UObject* ABC_Character::GetWeapon_Implementation()
{
	return EquippedWeapon.Get();
}
void ABC_Character::QuickAttack_Implementation()
{
	UE_LOG(Log_BC_Character, Display, TEXT("Character: %s made a quick attack!"), *GetName());
}
void ABC_Character::EquipWeapon_Implementation(UObject* Weapon)
{
	// If weapon is not valid or doesn't implement BC Weapon Interface, return
	if (!(Weapon && Weapon->Implements<UBC_WeaponInterface>()))
		return;
	
	EquippedWeapon = Cast<ABC_Weapon>(Weapon);
	EquippedWeapon.Get()->OnWeaponHit.AddDynamic(this, &ThisClass::OnWeaponHit);
}
void ABC_Character::UnequipWeapon_Implementation()
{
	EquippedWeapon->OnWeaponHit.RemoveDynamic(this, &ThisClass::OnWeaponHit);
	// EquippedWeapon = nullptr;
}
//~ End BC Attacker Interface

//~ Begin BC Damageable Interface
void ABC_Character::TakeDamage_Implementation(float Damage, const FHitResult& Hit)
{
	HitStop->HitStop();
	Attributes->DecreaseHealthBy(Damage);
	
	if (!Attributes->IsAlive())
	{
		HandleDeath();
		GetMesh()->AddImpulseAtLocation(-Hit.ImpactNormal * DeathImpulse, Hit.ImpactPoint);
	}

	UE_LOG(Log_BC_Character, Display, TEXT("Character: %s took %f damage. Health remaining: %f."), *GetName(), Damage, Attributes->GetHealth());
}
//~ End BC Damageable Interface



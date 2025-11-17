// Copyright Benski Game Works 2025, All rights reserved.


#include "GameFramework/BC_Character.h"

#include "Components/BC_AttributeComponent.h"
#include "Components/MontageComponent/BC_CharacterMontageComponent.h"
#include "Components/BC_HitStopComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Combat/BC_Weapon.h"
#include "Interfaces/BC_Interactable.h"

DEFINE_LOG_CATEGORY_STATIC(Log_BC_Character, All, All);

ABC_Character::ABC_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	MontageManager = CreateDefaultSubobject<UBC_CharacterMontageComponent>(TEXT("Montage Comp"));
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
	
	if (AActor* HitActor = Hit.GetActor(); HitActor->Implements<UBC_Damageable>())
	{
		Execute_TakeDamage(HitActor, this, EquippedWeapon.Get()->GetBaseDamage(), Hit);
	}
}

//~ Begin BC Attacker Interface
UObject* ABC_Character::GetWeapon_Implementation()
{
	return EquippedWeapon.Get();
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
	EquippedWeapon = nullptr;
}
//~ End BC Attacker Interface

//~ Begin BC Damageable Interface
void ABC_Character::TakeDamage_Implementation(AActor* Causer, float Damage, const FHitResult& Hit)
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

float ABC_Character::GetHealth() const
{
	return GetAttributes()->GetHealth();
}

bool ABC_Character::IsAlive() const
{
	return GetAttributes()->IsAlive();
}

float ABC_Character::GetHealthPercent() const
{
	return GetAttributes()->GetHealthPercent();
}

UBC_MontageComponent* ABC_Character::GetWeaponMontageManager() const
{
	if (ABC_Weapon* Weapon = EquippedWeapon.Get())
	{
		return Weapon->GetComponentByClass<UBC_MontageComponent>();
	}
	
	return nullptr;
}

void ABC_Character::PlayQuickAttackMontage()
{
	GetWeaponMontageManager()->PlayMontage(EBC_MontageType::EMT_QuickAttack);
}

void ABC_Character::PlayEquipMontage()
{
	GetWeaponMontageManager()->PlayMontage(EBC_MontageType::EMT_EquipWeapon, TEXT("Equip"));
}

void ABC_Character::PlayUnequipMontage()
{
	GetWeaponMontageManager()->PlayMontage(EBC_MontageType::EMT_EquipWeapon, TEXT("Unequip"));
}

UBC_MontageComponent* ABC_Character::GetMontageManager() const
{
	return MontageManager;
}

//~ End BC Damageable Interface

AActor* ABC_Character::GetNearestInteractable() const 
{
	AActor* NearestInteractable { nullptr };
	float NearestDistance { INFINITY };
	TSet<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);
	
	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor->Implements<UBC_Interactable>())
			continue;
		
		float Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
		if (Distance < NearestDistance)
		{
			NearestDistance = Distance;
			NearestInteractable = Actor;
		}
	}
	
	return NearestInteractable;
}


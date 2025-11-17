// Copyright Benski Game Works 2025, All rights reserved.


#include "Combat/BC_Weapon.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"
#include "Components/MontageComponent/BC_WeaponMontageComponent.h"
#include "Interfaces/BC_Damageable.h"


ABC_Weapon::ABC_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MontageManager = CreateDefaultSubobject<UBC_WeaponMontageComponent>(TEXT("Montage Comp"));
	
	WeaponType = EBC_WeaponType::EWT_None;
	BaseDamage = 25.0f;
	AttackTraceChannel = ECC_Visibility;
	bAttackTraceIgnoreOwner = true;
	bHitOncePerSwing = true;
	bDrawDebugAttackTrace = false;
	DrawDebugTime = 0.1f;
	bDebugHitsSeparate = true;
	DrawDebugHitsTime = 5.0f;
}
void ABC_Weapon::BeginPlay()
{
	Super::BeginPlay();
}
void ABC_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TickAttackLogic();
}

void ABC_Weapon::MakeActorsToIgnore(TArray<AActor*>& OutActorsToIgnore)
{
	OutActorsToIgnore.Add(this);
	
	if (APawn* OwnerPawn = GetInstigator(); bAttackTraceIgnoreOwner)
		OutActorsToIgnore.Add(OwnerPawn);
	
	if (bHitOncePerSwing)
	{
		for (TWeakObjectPtr<AActor> HitActor : CurrentHitActors)
		{
			if (AActor* AlreadyHitActor = HitActor.Get())
				OutActorsToIgnore.Add(AlreadyHitActor);
		}
	}
}
void ABC_Weapon::TickAttackLogic()
{
	TArray<AActor*> ActorsToIgnore; 
	MakeActorsToIgnore(ActorsToIgnore);

	// Do Attack Trace:
	FHitResult Hit;
	if (DoAttackTrace_Implementation(Hit, ActorsToIgnore))
	{
		AActor* HitActor = Hit.GetActor();

		// Block hitting the same actor if needed:
		if (bHitOncePerSwing)
		{
			if (CurrentHitActors.Contains(HitActor))
				return;
			
			CurrentHitActors.Add(HitActor);
		}

		// Broadcast Delegate
		OnWeaponHit.Broadcast(Hit);
	}
}
ABC_Weapon* ABC_Weapon::CreateWeapon(APawn* OwnerPawn, TSubclassOf<ABC_Weapon> WeaponClass)
{
	if (!ensureMsgf(OwnerPawn != nullptr, TEXT("Owner pawn is null.")))
		return nullptr;

	ABC_Weapon* NewWeapon = OwnerPawn->GetWorld()->SpawnActor<ABC_Weapon>(WeaponClass);
	check(NewWeapon != nullptr);

	NewWeapon->SetInstigator(OwnerPawn);

	return NewWeapon;
}

//~ Begin Weapon Interface
void ABC_Weapon::Attach_Implementation(USceneComponent* Parent, FName SocketName)
{
	AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}
void ABC_Weapon::BeginAttackTracing_Implementation()
{
	CurrentHitActors.Empty();
	SetActorTickEnabled(true);
}
void ABC_Weapon::EndAttackTracing_Implementation()
{
	SetActorTickEnabled(false);
	CurrentHitActors.Empty();
}
//~ End Weapon Interface




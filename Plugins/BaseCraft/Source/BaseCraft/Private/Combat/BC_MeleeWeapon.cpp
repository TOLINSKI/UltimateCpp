// Copyright Benski Game Works 2025, All rights reserved.


#include "Combat/BC_MeleeWeapon.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"
#include "Interfaces/BC_DamageableInterface.h"


ABC_MeleeWeapon::ABC_MeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	AttackTraceChannel = ECC_Visibility;
	bAttackTraceIgnoreOwner = true;
	bHitOncePerSwing = true;
	bDrawDebugAttackTrace = false;
	DrawDebugTime = 0.1f;
	bDebugHitsSeparate = true;
	DrawDebugHitsTime = 5.0f;
}

void ABC_MeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABC_MeleeWeapon::MakeActorsToIgnore(TArray<AActor*>& OutActorsToIgnore)
{
	OutActorsToIgnore.Add(this);
	
	if (APawn* Pawn = OwnerPawn.Get())
		OutActorsToIgnore.Add(Pawn);
	
	if (bHitOncePerSwing)
	{
		for (TWeakObjectPtr<AActor> WeakActorPtr : CurrentHitActors)
		{
			if (AActor* Actor = WeakActorPtr.Get())
			{
				OutActorsToIgnore.Add(Actor);
			}
		}
	}
}

void ABC_MeleeWeapon::TickAttackLogic()
{
	TArray<AActor*> ActorsToIgnore; 
	MakeActorsToIgnore(ActorsToIgnore);

	// Do Attack Trace:
	if (FHitResult Hit; DoAttackTrace_Implementation(Hit, ActorsToIgnore))
	{
		AActor* HitActor = Hit.GetActor();

		// Block hitting the same actor if needed:
		if (bHitOncePerSwing)
		{
			if (CurrentHitActors.Contains(HitActor))
				return;
			
			CurrentHitActors.Add(HitActor);
		}

		// Apply Damage to hit actor
		if (HitActor->Implements<UBC_DamageableInterface>())
		{
			FVector ImpactPoint = Hit.ImpactPoint;
			IBC_DamageableInterface::Execute_TakeDamage(HitActor, ImpactPoint);
		}

		// Broadcast Delegate
		OnMeleeWeaponHit.Broadcast(Hit);
	}
}

ABC_MeleeWeapon* ABC_MeleeWeapon::CreateWeapon(APawn* OwnerPawn, TSubclassOf<ABC_MeleeWeapon> WeaponClass)
{
	if (!ensureMsgf(OwnerPawn != nullptr, TEXT("Owner pawn is null.")))
		return nullptr;

	ABC_MeleeWeapon* NewWeapon = OwnerPawn->GetWorld()->SpawnActor<ABC_MeleeWeapon>(WeaponClass);
	check(NewWeapon != nullptr);

	NewWeapon->SetOwnerPawn(OwnerPawn);

	return NewWeapon;
}

void ABC_MeleeWeapon::Attach_Implementation(USceneComponent* Parent, FName SocketName)
{
	AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void ABC_MeleeWeapon::BeginAttackTracing_Implementation()
{
	CurrentHitActors.Empty();
	SetActorTickEnabled(true);
}

void ABC_MeleeWeapon::EndAttackTracing_Implementation()
{
	SetActorTickEnabled(false);
	CurrentHitActors.Empty();
}

void ABC_MeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TickAttackLogic();
}




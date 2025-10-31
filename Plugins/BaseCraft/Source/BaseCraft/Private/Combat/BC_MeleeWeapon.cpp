// Copyright Benski Game Works 2025, All rights reserved.


#include "Combat/BC_MeleeWeapon.h"
#include "Components/CapsuleComponent.h"
#include "kismet/KismetSystemLibrary.h"
#include "GameFramework/Pawn.h"

ABC_MeleeWeapon::ABC_MeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	AttackCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Attack Overlap"));
	AttackCapsule->SetupAttachment(Mesh);
	// Response to object type apparently : !!!
	AttackCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Overlap);
	// --------------------------------------------
	AttackCapsule->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(Mesh);

	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(Mesh);

	bCapsuleVisibleInEditor = true;
	bAttackcCapsuleIngoreOwner = true;
	AttackTraceChannel = ECC_Visibility;
}

void ABC_MeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	AttackCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABC_MeleeWeapon::OnAttackCapsuleBeginOverlap);
	
#if WITH_EDITOR
	if (bCapsuleVisibleInEditor)
		AttackCapsule->SetHiddenInGame(false);
#endif
}

void ABC_MeleeWeapon::OnAttackCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bAttackcCapsuleIngoreOwner
		&& OwnerPawn.IsValid()
		&& OwnerPawn == OtherActor)
		return;
	
	FHitResult Hit;
	if (Execute_DoAttackTrace(this, Hit))
		OnMeleeWeaponHit.Broadcast(Hit);
}

void ABC_MeleeWeapon::Attach_Implementation(USceneComponent* Parent, FName SocketName)
{
	AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void ABC_MeleeWeapon::EnableAttackCollision_Implementation()
{
	AttackCapsule->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}

void ABC_MeleeWeapon::DisableAttackCollision_Implementation()
{
	AttackCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

bool ABC_MeleeWeapon::DoAttackTrace_Implementation(FHitResult& HitResult)
{
	TArray<AActor*> ActorsToIgnore {this};
	if (APawn* Pawn = OwnerPawn.Get())
		ActorsToIgnore.Add(Pawn);

	ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(AttackTraceChannel);
	UE_LOG(LogTemp, Error, TEXT("%s"), *UEnum::GetValueAsString(TraceTypeQuery));
	
	bool bIsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		TraceStart->GetComponentLocation(),
		TraceEnd->GetComponentLocation(),
		AttackCapsule->GetScaledCapsuleRadius(),
		UEngineTypes::ConvertToTraceType(AttackTraceChannel),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true);

	return bIsHit;
}

void ABC_MeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


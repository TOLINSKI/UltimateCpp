// Copyright Benski Game Works 2025, All rights reserved.


#include "Combat/BC_MeleeWeapon.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"

ABC_MeleeWeapon::ABC_MeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
#if WITH_EDITORONLY_DATA
	AttackCapsulePreview = CreateDefaultSubobject<UCapsuleComponent>(TEXT("EDITOR ONLY"));
	AttackCapsulePreview->SetupAttachment(Root);
	AttackCapsulePreview->SetRelativeScale3D(FVector::OneVector);
	AttackCapsulePreview->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackCapsulePreview->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	AttackCapsulePreview->ShapeColor = FColor::Red;
#endif
	
	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(Root);

	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(Root);

	AttackTraceRadius = 16.0f;
	AttackTraceChannel = ECC_Visibility;
	bAttackTraceIgnoreOwner = true;
	bDrawDebugAttackTrace = false;
	DrawDebugTime = 0.2f;
}

#if WITH_EDITORONLY_DATA
void ABC_MeleeWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (AttackCapsulePreview && TraceStart && TraceEnd)
	{
		AttackCapsulePreview->SetCapsuleHalfHeight(GetTraceCapsuleHalfHeight());
		AttackCapsulePreview->SetRelativeLocation((TraceEnd->GetRelativeLocation() + TraceStart->GetRelativeLocation()) / 2.0f);
		AttackCapsulePreview->SetCapsuleRadius(AttackTraceRadius);
		AttackCapsulePreview->SetRelativeRotation(GetTraceCapsuleRotation());
	}
}
#endif

void ABC_MeleeWeapon::DrawDebugAttackCapsule(const FColor& Color)
{
	DrawDebugCapsule(
		GetWorld(),
		(TraceEnd->GetComponentLocation() + TraceStart->GetComponentLocation()) * 0.5f,
		GetTraceCapsuleHalfHeight(),
		AttackTraceRadius,
		TraceStart->GetComponentQuat(),
		Color,
		false,
		DrawDebugTime);
}

FVector ABC_MeleeWeapon::GetTraceRadiusStep()
{
	if (TraceStart && TraceEnd)
	{
		FVector Direction = (TraceEnd->GetComponentLocation() - TraceStart->GetComponentLocation()).GetSafeNormal();
		return Direction * AttackTraceRadius;
	}
	return FVector::OneVector;
}

float ABC_MeleeWeapon::GetTraceCapsuleHalfHeight()
{
	if (TraceStart && TraceEnd)
		return FVector::Dist(TraceEnd->GetComponentLocation() + GetTraceRadiusStep(), TraceStart->GetComponentLocation() - GetTraceRadiusStep())/ 2.0f;

	return 22.0f;
}

FQuat ABC_MeleeWeapon::GetTraceCapsuleRotation()
{
	if (TraceStart && TraceEnd)
		return FQuat::FindBetweenVectors(TraceStart->GetComponentLocation(), TraceEnd->GetComponentLocation());

	return FQuat::Identity;
}

void ABC_MeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABC_MeleeWeapon::Attach_Implementation(USceneComponent* Parent, FName SocketName)
{
	AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void ABC_MeleeWeapon::EnableAttackCollision_Implementation()
{
	SetActorTickEnabled(true);
	bAttackEnabled = true;
}

void ABC_MeleeWeapon::DisableAttackCollision_Implementation()
{
	SetActorTickEnabled(false);
	bAttackEnabled = false;
}

bool ABC_MeleeWeapon::DoAttackTrace_Implementation(FHitResult& HitResult)
{
	FCollisionShape Sphere { FCollisionShape::MakeSphere(AttackTraceRadius) };
	
	TArray<AActor*> ActorsToIgnore {this};
	if (APawn* Pawn = OwnerPawn.Get())
		ActorsToIgnore.Add(Pawn);
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(ActorsToIgnore);

	bool bIsHit = GetWorld()->SweepMultiByChannel(
		Hits,
		TraceStart->GetComponentLocation(),
		TraceEnd->GetComponentLocation(),
		FQuat::Identity,
		AttackTraceChannel,
		Sphere,
		CollisionParams,
		FCollisionResponseParams::DefaultResponseParam);

#if WITH_EDITOR
	if (bDrawDebugAttackTrace)
		DrawDebugAttackCapsule(bIsHit ? FColor::Green : FColor::Red);
#endif		
	
	return bIsHit;
}

void ABC_MeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FHitResult FakeHit;

	if (bAttackEnabled && DoAttackTrace_Implementation(FakeHit))
	{
		SetActorTickEnabled(false);
		bAttackEnabled = false;
		OnMeleeWeaponHit.Broadcast(Hits[0]);
	}
}




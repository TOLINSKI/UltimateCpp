// Copyright Benski Game Works 2025, All rights reserved.


#include "Combat/Melee/BC_CapsuleWeapon.h"
#include "KismetTraceUtils.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/BC_Damageable.h"


ABC_CapsuleWeapon::ABC_CapsuleWeapon()
{
	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(Root);

	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(Root);

	AttackTraceRadius = 16.0f;
	
#if WITH_EDITORONLY_DATA
	AttackCapsulePreview = CreateDefaultSubobject<UCapsuleComponent>(TEXT("EDITOR ONLY"));
	AttackCapsulePreview->SetupAttachment(Root);
	AttackCapsulePreview->SetRelativeScale3D(FVector::OneVector);
	AttackCapsulePreview->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackCapsulePreview->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	AttackCapsulePreview->ShapeColor = FColor::Red;
#endif
}

#if WITH_EDITORONLY_DATA
void ABC_CapsuleWeapon::OnConstruction(const FTransform& Transform)
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

bool ABC_CapsuleWeapon::DoAttackTrace_Implementation(FHitResult& OutHit, const TArray<AActor*>& ActorsToIgnore)
{
	FCollisionShape Sphere { FCollisionShape::MakeSphere(AttackTraceRadius) };
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(ActorsToIgnore);

	bool bIsHit = GetWorld()->SweepSingleByChannel(
		OutHit,
		TraceStart->GetComponentLocation(),
		TraceEnd->GetComponentLocation(),
		FQuat::Identity,
		AttackTraceChannel,
		Sphere,
		CollisionParams,
		FCollisionResponseParams::DefaultResponseParam
		);

#if WITH_EDITOR
	DrawDebugAttackCapsule(bIsHit, OutHit);
#endif
	
	return bIsHit;
}

//~ Begin Debug
FVector ABC_CapsuleWeapon::GetTraceRadiusStep() const
{
	if (TraceStart && TraceEnd)
	{
		FVector Direction = (TraceEnd->GetComponentLocation() - TraceStart->GetComponentLocation()).GetSafeNormal();
		return Direction * AttackTraceRadius;
	}
	return FVector::OneVector;
}
float ABC_CapsuleWeapon::GetTraceCapsuleHalfHeight() const
{
	if (TraceStart && TraceEnd)
		return FVector::Dist(TraceEnd->GetComponentLocation() + GetTraceRadiusStep(), TraceStart->GetComponentLocation() - GetTraceRadiusStep())/ 2.0f;

	return 22.0f;
}
FQuat ABC_CapsuleWeapon::GetTraceCapsuleRotation() const
{
	if (TraceStart && TraceEnd)
	{
		FVector TraceDirection = (TraceEnd->GetComponentLocation() - TraceStart->GetComponentLocation()).GetSafeNormal();
		// Find the roatation needed between up vector and the direction between trace-start to trace-end.
		// >> The capsule is "standing up" (aligned with up vector) by default.
		return FQuat::FindBetweenVectors(FVector::UpVector, TraceDirection); 
	}
	
	return FQuat::Identity;
}
#if WITH_EDITOR
void ABC_CapsuleWeapon::DrawDebugAttackCapsule(const bool bIsHit, const FHitResult& HitResult) const
{
	if (bDrawDebugAttackTrace)
		DrawDebugSphereTraceSingle(
		GetWorld(),
		TraceStart->GetComponentLocation(),
		TraceEnd->GetComponentLocation(),
		AttackTraceRadius,
		DrawDebugTime <= 0.0f ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::ForDuration,
		bIsHit,
		HitResult,
		FLinearColor::Red,
		FLinearColor::Green,
		bDebugHitsSeparate && bIsHit ? DrawDebugHitsTime : DrawDebugTime);
}
#endif
//~ End Debug
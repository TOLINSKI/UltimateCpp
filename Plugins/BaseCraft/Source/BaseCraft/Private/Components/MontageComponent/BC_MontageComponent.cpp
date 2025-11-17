// Copyright Benski Game Works 2025, All rights reserved.


#include "Components/MontageComponent/BC_MontageComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

using enum EBC_MontageType;

UBC_MontageComponent::UBC_MontageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	bMontagesLoaded = false;
}

void UBC_MontageComponent::BeginPlay()
{
	Super::BeginPlay();
	
	LoadMontages();
}

void UBC_MontageComponent::LoadMontages()
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	FStreamableDelegate StreamableDelegate;
	StreamableDelegate.BindUObject(this, &UBC_MontageComponent::OnMontagesLoaded);

	TArray<FSoftObjectPath> SoftObjectPaths;
	GetMontagesSoftPaths(SoftObjectPaths);
	
	StreamableManager.RequestAsyncLoad(SoftObjectPaths, StreamableDelegate);
}

FName UBC_MontageComponent::GetHitReactMontageSectionName(const FVector& ImpactPoint) const
{
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	const FVector ToImpact = ((ImpactPoint - OwnerLocation)*FVector(1.0f, 1.0f, 0.0f)).GetSafeNormal();
	const FVector Forward = GetOwner()->GetActorForwardVector();
	const float Angle = FMath::Acos(Forward.Dot(ToImpact)); // Always positive!
	const float CrossZ = Forward.Cross(ToImpact).Z; // Positive when to the right of forward
	
	// Angle is always positive, use cross Z to determine sign
	if (Angle <= UE_PI*0.25f)
		return TEXT("FromFront");
	if (Angle >= UE_PI*0.75f)
		return TEXT("FromBack");
	if (CrossZ < 0.0f)
		return TEXT("FromLeft");
	if (CrossZ > 0.0f)
		return TEXT("FromRight");

	return TEXT("FromFront");
}




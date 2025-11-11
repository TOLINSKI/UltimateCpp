// Copyright Benski Game Works 2025, All rights reserved.


#include "Components/BC_HoverComponent.h"


UBC_HoverComponent::UBC_HoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = true;
	
	// Hovering:
	Amplitude = 5.0f;
	Frequency = 0.5f;
	HoverDirection = FVector::UpVector;
	HoverFunc = EBC_HoverFunc::Sin;
}

void UBC_HoverComponent::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetOwner()->GetActorLocation();

	if (bAutoActivate)
		SetComponentTickEnabled(true);
}

void UBC_HoverComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsActive())
		Hover(DeltaTime);
}

void UBC_HoverComponent::SetActive(bool bNewActive, bool bReset)
{
	Super::SetActive(bNewActive, bReset);

	SetComponentTickEnabled(bNewActive);
}

//~ Begin Hovering
void UBC_HoverComponent::Hover(float DeltaTime)
{
	RunningTime += DeltaTime;

	float Offset = 0.0f;
	switch (HoverFunc)
	{
	case EBC_HoverFunc::Cos:
		Offset = Amplitude * FMath::Cos(RunningTime * TWO_PI * Frequency);
		break;
	case EBC_HoverFunc::Sin:
		Offset = Amplitude * FMath::Sin(RunningTime * TWO_PI * Frequency);
		break;
	default:
		break;
	}

	GetOwner()->AddActorWorldOffset(HoverDirection.GetSafeNormal() * Offset);
}
//~ End Hovering

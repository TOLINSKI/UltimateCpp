// Copyright Benski Game Works 2025, All rights reserved.


#include "Components/BC_HitStopComponent.h"


UBC_HitStopComponent::UBC_HitStopComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	TimeDilation = 0.025;
	HitStopDuration = 0.08f;
}

void UBC_HitStopComponent::HitStop()
{
	if (FMath::IsNearlyZero(HitStopDuration))
		return;
	
	GetOwner()->CustomTimeDilation = TimeDilation;
	GetWorld()->GetTimerManager().SetTimer(HitStopTimer, this, &UBC_HitStopComponent::HitStopTimeout, HitStopDuration, false);
}

void UBC_HitStopComponent::HitStopTimeout() const
{
	GetOwner()->CustomTimeDilation = 1.0f;
}




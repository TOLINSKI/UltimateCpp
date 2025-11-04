// Copyright Benski Game Works 2025, All rights reserved.


#include "Components/BC_HitStopComponent.h"


UBC_HitStopComponent::UBC_HitStopComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UBC_HitStopComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UBC_HitStopComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


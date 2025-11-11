// Copyright Benski Game Works 2025, All rights reserved.


#include "Components/BC_AttributeComponent.h"


UBC_AttributeComponent::UBC_AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
	Health = MaxHealth;
}


void UBC_AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

float UBC_AttributeComponent::DecreaseHealthBy(float HealthToRemove)
{
	Health = FMath::Max(0.0f, Health - HealthToRemove);
	return Health;
}

float UBC_AttributeComponent::IncreaseHealthBy(float HealthToAdd)
{
	Health = FMath::Min(MaxHealth, Health + HealthToAdd);
	return Health;
}



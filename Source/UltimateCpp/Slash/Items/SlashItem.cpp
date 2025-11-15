// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashItem.h"

#include "Components/BC_HoverComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"


ASlashItem::ASlashItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASlashItem::BeginPlay()
{
	Super::BeginPlay();
}

void ASlashItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlashItem::EnablePickup()
{
	PickupSphere->SetGenerateOverlapEvents(true);
	HoveringMovement->SetActive(true);
	RotatingMovement->SetActive(true);
}

void ASlashItem::DisablePickup()
{
	PickupSphere->SetGenerateOverlapEvents(false);
	HoveringMovement->SetActive(false);
	RotatingMovement->SetActive(false);
}


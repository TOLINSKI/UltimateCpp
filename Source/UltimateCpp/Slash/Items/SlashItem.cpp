// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashItem.h"

#include "SlashCharacter.h"
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
	BC_Hover->SetActive(true);
	RotatingMovement->SetActive(true);
}

void ASlashItem::DisablePickup()
{
	PickupSphere->SetGenerateOverlapEvents(false);
	BC_Hover->SetActive(false);
	RotatingMovement->SetActive(false);
}

void ASlashItem::OnPickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnPickupSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (ASlashCharacter* Character = Cast<ASlashCharacter>(OtherActor))
	{
		// Set self as latest interactable
		Character->SetInteractable(this);
	}
}

void ASlashItem::OnPickupSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnPickupSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
	if (ASlashCharacter* Character = Cast<ASlashCharacter>(OtherActor))
	{
		// First check if this is the latest interactable, if so, set nullptr.
		AActor* CurrentItem = Character->GetInteractable();
		if (CurrentItem && CurrentItem == this)
		{
			Character->SetInteractable(nullptr);
		}
	}
}


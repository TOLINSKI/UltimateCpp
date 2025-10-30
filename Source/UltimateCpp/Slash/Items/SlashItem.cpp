// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashItem.h"

#include "SlashCharacter.h"


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

void ASlashItem::OnPickupShapeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnPickupShapeBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (ASlashCharacter* Character = Cast<ASlashCharacter>(OtherActor))
	{
		// Set self as latest interactable
		Character->SetInteractable(this);
	}
}

void ASlashItem::OnPickupShapeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnPickupShapeEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
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


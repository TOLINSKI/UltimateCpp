// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/BC_Item.h"
#include "SlashItem.generated.h"

UCLASS(Abstract)
class ULTIMATECPP_API ASlashItem : public ABC_Item
{
	GENERATED_BODY()

public:
	ASlashItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void EnablePickup();
	virtual void DisablePickup();
	
// Slash Interface
protected:
	virtual void OnPickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual void OnPickupSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};

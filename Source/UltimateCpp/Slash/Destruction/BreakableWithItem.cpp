// Copyright Benski Game Works 2025, All rights reserved.


#include "BreakableWithItem.h"
#include "Items/BC_Item.h"

// Sets default values
ABreakableWithItem::ABreakableWithItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	ItemPosition->SetupAttachment(RootComponent);
}

void ABreakableWithItem::BeginPlay()
{
	Super::BeginPlay();

	Item = GetWorld()->SpawnActor<ABC_Item>(ItemClass, ItemPosition->GetComponentLocation(), FRotator::ZeroRotator);
	Item->SetPickupEnabled(false);
}

void ABreakableWithItem::OnBreak(const FChaosBreakEvent& BreakEvent)
{
	Super::OnBreak(BreakEvent);

	if (Item.Get())
	{
		Item->SetPickupEnabled(true);
		Item = nullptr;
	}
}



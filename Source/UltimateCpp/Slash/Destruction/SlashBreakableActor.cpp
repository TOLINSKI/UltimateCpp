// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashBreakableActor.h"
#include "Components/SceneComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"


ASlashBreakableActor::ASlashBreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GC"));
	GeometryCollection->SetupAttachment(Root);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryCollection->SetNotifyBreaks(true);

	bIsBroken = false;
}

void ASlashBreakableActor::BeginPlay()
{
	Super::BeginPlay();

	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ThisClass::OnBreak);
}

void ASlashBreakableActor::OnBreak(const FChaosBreakEvent& BreakEvent)
{
	if (bIsBroken)
		return;

	bIsBroken = true;
	GeometryCollection->SetCollisionResponseToAllChannels(ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GeometryCollection->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	SetLifeSpan(LifeSpan);
}




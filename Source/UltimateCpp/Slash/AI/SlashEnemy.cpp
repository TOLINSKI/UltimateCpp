// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashEnemy.h"

#include "Components/CapsuleComponent.h"


ASlashEnemy::ASlashEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);	
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ASlashEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlashEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlashEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


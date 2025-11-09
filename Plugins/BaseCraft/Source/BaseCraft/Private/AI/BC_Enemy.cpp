// Copyright Benski Game Works 2025, All rights reserved.


#include "AI/BC_Enemy.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(Log_BC_Enemy, All, All);

// Sets default values
ABC_Enemy::ABC_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
}



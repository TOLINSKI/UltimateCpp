// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashTreasure.h"
#include "NiagaraComponent.h"
#include "Components/BC_InteractButtonComponent.h"
#include "Slash/SlashCharacter.h"


ASlashTreasure::ASlashTreasure()
{
	PrimaryActorTick.bCanEverTick = true;
	Gold = 5;
}

void ASlashTreasure::BeginPlay()
{
	Super::BeginPlay();
}

void ASlashTreasure::Interact_Implementation(AActor* Interactor)
{
	Super::Interact_Implementation(Interactor);

	if (ASlashCharacter* Character = Cast<ASlashCharacter>(Interactor))
	{
		bCanInteract = false;
		InteractWidget->HideButton();
		Destroy();
	}
}


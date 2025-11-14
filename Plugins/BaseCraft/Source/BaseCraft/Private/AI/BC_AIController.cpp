// Copyright Benski Game Works 2025, All rights reserved.


#include "AI/BC_AIController.h"
#include "Components/StateTreeAIComponent.h"

ABC_AIController::ABC_AIController()
{
	PrimaryActorTick.bCanEverTick = true;

	StateTreeAI = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAI"));
	check(StateTreeAI);

	bStartAILogicOnPossess = true;
	bAttachToPawn = true;
}



// Copyright Benski Game Works 2025, All rights reserved.


#include "AI/BC_Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BC_AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "AI/BC_AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "Perception/AISense_Sight.h"
#include "GameplayTagsManager.h"
#include "GameFramework/BC_PlayerCharacter.h"
#include "kismet/GameplayStatics.h"
#include "UI/BC_HealthBarWidget.h"

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

	// Orient rotation to movement, disable control rotation:
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Comp"));
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Comp"));
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBar->SetDrawAtDesiredSize(false);
	HealthBar->SetDrawSize(FVector2D(250.0f, 25.0f));
	HealthBar->SetVisibility(false); // Wait to show health on command

	LifeSpan = 3.0f;
}

void ABC_Enemy::BeginPlay()
{
	Super::BeginPlay();

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ABC_Enemy::OnAIPerceptionTargetUpdated);
	
	if (UBC_HealthBarWidget* HealthBarWidget = Cast<UBC_HealthBarWidget>(HealthBar->GetUserWidgetObject()))
	{
		HealthBarWidget->SetHealthPercent(GetAttributes()->GetHealthPercent());
	}
}

void ABC_Enemy::TakeDamage_Implementation(AActor* Causer, float Damage, const FHitResult& Hit)
{
	Super::TakeDamage_Implementation(Causer, Damage, Hit);

	if (UBC_HealthBarWidget* HealthBarWidget = Cast<UBC_HealthBarWidget>(HealthBar->GetUserWidgetObject()))
	{
		HealthBarWidget->SetHealthPercent(GetAttributes()->GetHealthPercent());
	}

	if (Causer->IsA<ABC_PlayerCharacter>() && GetAttributes()->IsAlive())
	{
		SetShowHealthBar(true);	
		
		// SendStateTreeEvent(TEXT("AI.SeePlayer"));
		TArray<AActor*> Enemies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABC_Enemy::StaticClass(), Enemies);

		for (auto Enemy : Enemies)
		{
			if (FVector::Dist(GetActorLocation(), Enemy->GetActorLocation()) < 1000.0f)
			{
				Cast<ABC_Enemy>(Enemy)->SendStateTreeEvent(TEXT("AI.SeePlayer"));
			}
		}
	}
}

void ABC_Enemy::SendStateTreeEvent(const FName& GameplayTagName)
{
	if (ABC_AIController* AIController = Cast<ABC_AIController>(GetController()))
	{
		if (UStateTreeAIComponent* StateTreeAI = AIController->GetStateTreeAI())
		{
			const FGameplayTag GameplayTag = UGameplayTagsManager::Get().RequestGameplayTag(GameplayTagName);
			const FStateTreeEvent StateTreeEvent(GameplayTag);
			StateTreeAI->SendStateTreeEvent(StateTreeEvent);
		}
	}
}

void ABC_Enemy::HandleDeath()
{
	Super::HandleDeath();
	
	// hide the life bar
	SetShowHealthBar(false);

	// Notify State Tree
	SendStateTreeEvent(TEXT("AI.Died"));
}

void ABC_Enemy::OnAIPerceptionTargetUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (ABC_Character* Player = Cast<ABC_Character>(Actor); Player->IsPlayerControlled() && Player->GetAttributes()->IsAlive())
	{
		TArray<AActor*> SensedActors;
		TSubclassOf<UAISense> SenseClass;
		AIPerception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SensedActors);

		if (SensedActors.Contains(Player))
		{
			SendStateTreeEvent(TEXT("AI.SeePlayer"));
		}
		else if (FVector::Dist(Player->GetActorLocation(), GetActorLocation()) > 800.0f)
		{
			// When player is not perceived and far enough
			SendStateTreeEvent(TEXT("AI.LostPlayer"));
			SetShowHealthBar(false);
		}
		else
		{
			SetShowHealthBar(false);
		}
	}
}

AActor* ABC_Enemy::GetNextPatrolTarget()
{
	int32 NumTargets = PatrolTargets.Num();

	// Return if no patrol targets
	if (NumTargets == 0)
		return nullptr;

	// Randomly choose and remove a patrol target from array
	AActor* NewPatrolTarget = PatrolTargets[FMath::RandRange(0, NumTargets - 1)];
	PatrolTargets.Remove(NewPatrolTarget);
	
	// Place current target into array
	if (PatrolTarget)
		PatrolTargets.Add(PatrolTarget);

	// Store reference to the "popped" patrol target
	PatrolTarget = NewPatrolTarget; 
	return PatrolTarget;
}

void ABC_Enemy::SetShowHealthBar(bool bShowHealthBar)
{
	HealthBar->SetVisibility(bShowHealthBar);
}





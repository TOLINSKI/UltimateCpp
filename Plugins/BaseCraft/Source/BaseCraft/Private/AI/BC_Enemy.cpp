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
#include "Combat/BC_Weapon.h"
#include "Components/MontageComponent/BC_MontageComponent.h"
#include "GameFramework/BC_PlayerCharacter.h"
#include "kismet/GameplayStatics.h"
#include "UI/BC_HealthBarWidget.h"
#include "kismet/KismetMathLibrary.h"

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

	//~ State
	LifeSpan = 3.0f;
	EnemyState = EBC_AIState::EES_Idle;
	
	//~ Combat
	CombatSensingRadius = 1000.0f;
	RotateToTargetSpeed = 2.0f;	
	RotateToTargetThreshold = 15.0f;
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

	if (GetAttributes()->IsAlive())
	{
		if (Causer->IsA<ABC_PlayerCharacter>())
		{
			SetShowHealthBar(true);	
		
			// SendStateTreeEvent(TEXT("AI.SeePlayer"));
			TArray<AActor*> Enemies;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABC_Enemy::StaticClass(), Enemies);
		
			for (auto EnemyActor : Enemies)
			{
				if (FVector::Dist(GetActorLocation(), EnemyActor->GetActorLocation()) <= CombatSensingRadius)
				{
					Cast<ABC_Enemy>(EnemyActor)->SetCombatTarget(Causer);
				}
			}
		}
		else
		{
			SetCombatTarget(Causer);
		}
	}
}

bool ABC_Enemy::RotateToTarget(AActor* Target, float DeltaTime)
{
	if (Target == nullptr && !GetCombatTarget())
		return true;
	
	if (Target == nullptr)
		Target = GetCombatTarget();
	
	FVector EnemyLocation = GetActorLocation();
	FVector CombatTargetLocation = Target->GetActorLocation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(EnemyLocation, CombatTargetLocation);
	
	FRotator NewRotation = FMath::RInterpTo(
		GetActorRotation(), 
		LookAtRotation,
		DeltaTime,
		RotateToTargetSpeed);
	
	SetActorRotation(NewRotation);
	
	return NewRotation.Equals(LookAtRotation, RotateToTargetThreshold);
}

bool ABC_Enemy::IsAttacking()
{
	if (ABC_Weapon* Weapon = EquippedWeapon.Get())
	{
		if (UBC_MontageComponent* WeaponMontages = Weapon->GetComponentByClass<UBC_MontageComponent>())
		{
			return WeaponMontages->IsPlayingMontage(EBC_MontageType::EMT_QuickAttack);
		}
	}
	
	return  false;
}

void ABC_Enemy::QuickAttack()
{
	UE_LOG(Log_BC_Enemy, Display, TEXT("Enemy: %s is attacking."), *GetName());
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

void ABC_Enemy::LooseCombatTarget()
{
	// When player is not perceived and far enough
	CombatTarget = nullptr;
	SendStateTreeEvent(TEXT("AI.CombatTargetLost"));
	SetShowHealthBar(false);
	EnemyState = EBC_AIState::EES_Patrol;
}

void ABC_Enemy::AcquireCombatTarget(AActor* NewCombatTarget)
{
	CombatTarget = NewCombatTarget;
	SendStateTreeEvent(TEXT("AI.CombatTargetAcquired"));
	EnemyState = EBC_AIState::EES_Combat;
}

void ABC_Enemy::SetCombatTarget(AActor* NewCombatTarget)
{
	AcquireCombatTarget(NewCombatTarget);
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
			AcquireCombatTarget(Player);
		}
		else if (FVector::Dist(Player->GetActorLocation(), GetActorLocation()) > CombatSensingRadius)
		{
			LooseCombatTarget();
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

bool ABC_Enemy::IsTakingDamage()
{
	return GetMontageManager()->IsPlayingMontage(EBC_MontageType::EMT_HitReact);
}

void ABC_Enemy::SetShowHealthBar(bool bShowHealthBar)
{
	HealthBar->SetVisibility(bShowHealthBar);
}





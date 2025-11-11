// Copyright Benski Game Works 2025, All rights reserved.


#include "SlashEnemy.h"

#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BC_AttributeComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSlashEnemy, All, All);

ASlashEnemy::ASlashEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	Attributes = CreateDefaultSubobject<UBC_AttributeComponent>(TEXT("Attribute Comp"));
}

void ASlashEnemy::TakeDamage_Implementation(const FVector& ImpactPoint, float Damage)
{
	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	
	if (HitParticles)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitParticles, ImpactPoint);

	Attributes->DecreaseHealthBy(Damage);

	UE_LOG(LogSlashEnemy, Display, TEXT("Health to remove: %f"), Damage);
	UE_LOG(LogSlashEnemy, Display, TEXT("Health left: %f"), Attributes->GetHealth());
	
	PlayHitReactMontage(ImpactPoint);
}

FName ASlashEnemy::GetHitReactMontageSectionName(const FVector& ImpactPoint) const
{
	const FVector ToImpact = ((ImpactPoint - GetActorLocation())*FVector(1.0f, 1.0f, 0.0f)).GetSafeNormal();
	const FVector Forward = GetActorForwardVector();
	const float Angle = FMath::Acos(Forward.Dot(ToImpact)); // Always positive!
	const float CrossZ = Forward.Cross(ToImpact).Z; // Positive when to the right of forward
	
	UE_LOG(LogSlashEnemy, Display, TEXT("Angle: %f"), Angle);

	// Angle is always positive, use cross Z to determine sign
	if (Angle <= UE_PI*0.25f)
		return TEXT("FromFront");
	if (Angle >= UE_PI*0.75f)
		return TEXT("FromBack");
	if (CrossZ < 0.0f)
		return TEXT("FromLeft");
	if (CrossZ > 0.0f)
		return TEXT("FromRight");

	return TEXT("FromFront");
}

void ASlashEnemy::PlayHitReactMontage(const FVector& ImpactPoint) const
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); HitReactMontage != nullptr)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		FName SectionName = GetHitReactMontageSectionName(ImpactPoint);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}




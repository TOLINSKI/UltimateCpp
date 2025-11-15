// Copyright Benski Game Works 2025, All rights reserved.


#include "Items/BC_Item.h"

#include "NiagaraComponent.h"
#include "Components/BC_HoverComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/BC_InteractButtonComponent.h"
#include "kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSlashItem, All, All);

ABC_Item::ABC_Item()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(Root);
	
	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	PickupSphere->SetupAttachment(RootComponent);
	PickupSphere->bHiddenInGame = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	GlowEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Comp"));
	GlowEffect->SetupAttachment(RootComponent);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Comp"));

	// Base Craft Components:
	InteractWidget = CreateDefaultSubobject<UBC_InteractButtonComponent>(TEXT("Widget Comp"));
	InteractWidget->SetupAttachment(Root);
	
	HoveringMovement = CreateDefaultSubobject<UBC_HoverComponent>(TEXT("Hovering Comp"));

	bCanInteract = true;
}

void ABC_Item::BeginPlay()
{
	Super::BeginPlay();

	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::ABC_Item::OnPickupSphereBeginOverlap);
	PickupSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnPickupSphereEndOverlap);
}

void ABC_Item::OnPickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
#if WITH_EDITOR
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("%s Began Overlap with: %s"), *GetName(), *OtherActor->GetName()));
	UE_LOG(LogSlashItem, Display, TEXT("%s Began Overlap with: %s"), *GetName(), *OtherActor->GetName());
#endif
	
	if (bCanInteract)
		InteractWidget->ShowButton();
}

void ABC_Item::OnPickupSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
#if WITH_EDITOR
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("%s Ended Overlap with: %s"), *GetName(), *OtherActor->GetName()));
	UE_LOG(LogSlashItem, Display, TEXT("%s Ended Overlap with: %s"), *GetName(), *OtherActor->GetName());
#endif
	
	InteractWidget->HideButton();
}

void ABC_Item::Interact_Implementation(AActor* InstigatorActor)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("%s Interacts with: %s"), *InstigatorActor->GetName(), *GetName()));
	UE_LOG(LogSlashItem, Display, TEXT("%s Interacts with: %s"), *InstigatorActor->GetName(), *GetName());

	if (InteractionSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractionSound, GetActorLocation());
}

bool ABC_Item::CanInteract_Implementation()
{
	return bCanInteract;
}


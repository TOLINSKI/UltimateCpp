// Copyright Benski Game Works 2025, All rights reserved.


#include "Items/BC_Item.h"

#include "Components/BC_HoverComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/BC_InteractButtonComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSlashItem, All, All);

ABC_Item::ABC_Item()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Sphere"));
	PickupSphere->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement"));

	// Base Craft Components:
	BC_ButtonWidget = CreateDefaultSubobject<UBC_InteractButtonComponent>(TEXT("Interact Button Widget"));
	BC_ButtonWidget->SetupAttachment(Mesh);
	
	BC_Hover = CreateDefaultSubobject<UBC_HoverComponent>(TEXT("Hovering Movement"));

	bShowPickupSphereInEditor = true;
	bCanInteract = true;
}

void ABC_Item::BeginPlay()
{
	Super::BeginPlay();

	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::ABC_Item::OnPickupSphereBeginOverlap);
	PickupSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnPickupSphereEndOverlap);

#if WITH_EDITOR	
	PickupSphere->SetHiddenInGame(!bShowPickupSphereInEditor);
#endif
}

void ABC_Item::OnPickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
#if WITH_EDITOR
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("%s Began Overlap with: %s"), *GetName(), *OtherActor->GetName()));
	UE_LOG(LogSlashItem, Display, TEXT("%s Began Overlap with: %s"), *GetName(), *OtherActor->GetName());
#endif
	
	if (bCanInteract)
		BC_ButtonWidget->ShowButton();
}

void ABC_Item::OnPickupSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
#if WITH_EDITOR
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("%s Ended Overlap with: %s"), *GetName(), *OtherActor->GetName()));
	UE_LOG(LogSlashItem, Display, TEXT("%s Ended Overlap with: %s"), *GetName(), *OtherActor->GetName());
#endif
	
	BC_ButtonWidget->HideButton();
}

void ABC_Item::Interact_Implementation(AActor* InstigatorActor)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("%s Interacts with: %s"), *InstigatorActor->GetName(), *GetName()));
	UE_LOG(LogSlashItem, Display, TEXT("%s Interacts with: %s"), *InstigatorActor->GetName(), *GetName());
}


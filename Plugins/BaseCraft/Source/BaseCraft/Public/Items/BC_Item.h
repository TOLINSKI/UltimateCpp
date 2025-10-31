// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/BC_Interactable.h"
#include "BC_Item.generated.h"

class URotatingMovementComponent;
class USceneComponent;
class USphereComponent;
class UBC_HoverComponent;
class UStaticMeshComponent;
class UBC_InteractButtonComponent;

UCLASS(BlueprintType, Blueprintable)
class BASECRAFT_API ABC_Item : public AActor, public IBC_Interactable
{
	GENERATED_BODY()

// UE Interface
public:
	ABC_Item();
	
protected:
	virtual void BeginPlay() override;
	
// Base Craft Interface
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<USphereComponent> PickupSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<UBC_HoverComponent> BC_Hover;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<URotatingMovementComponent> RotatingMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Components")
	TObjectPtr<UBC_InteractButtonComponent> BC_ButtonWidget;
	
	/** ONLY IN EDITOR: Will change HiddenInGame for the Pickup sphere. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft")
	bool bShowPickupSphereInEditor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Interaction")
	bool bCanInteract;
	
	UFUNCTION()
	virtual void OnPickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnPickupSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	virtual void Interact_Implementation(AActor* InstigatorActor) override;
};

// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/BC_Item.h"
#include "BC_ItemWeapon.generated.h"

class ABC_Weapon;

UCLASS()
class BASECRAFT_API ABC_ItemWeapon : public ABC_Item
{
	GENERATED_BODY()

public:
	ABC_ItemWeapon();
	
// Slash Weapon Interface
public:
	UFUNCTION(BlueprintCallable, Category="Slash|Weapon")
	void Equip(APawn* WeaponOwner);
	
protected:
	virtual void Interact_Implementation(AActor* InstigatorActor) override;

	/** The actual weapon class to spawn when collected. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slash|Weapon")
	TSubclassOf<ABC_Weapon> WeaponClass;
};

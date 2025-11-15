// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/SlashItem.h"
#include "SlashItem_Weapon.generated.h"

class ASlashCharacter;
class ASlashWeapon;

UCLASS()
class ULTIMATECPP_API ASlashItem_Weapon : public ASlashItem
{
	GENERATED_BODY()

public:
	ASlashItem_Weapon();
	
// Slash Weapon Interface
public:
	UFUNCTION(BlueprintCallable, Category="Slash|Weapon")
	void Equip(APawn* WeaponOwner);
	
protected:
	virtual void Interact_Implementation(AActor* InstigatorActor) override;

	/** The actual weapon class to spawn when collected. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slash|Weapon")
	TSubclassOf<ASlashWeapon> WeaponClass;
};

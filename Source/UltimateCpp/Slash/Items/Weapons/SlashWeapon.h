// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/SlashItem.h"
#include "SlashWeapon.generated.h"

class ASlashCharacter;

UCLASS()
class ULTIMATECPP_API ASlashWeapon : public ASlashItem
{
	GENERATED_BODY()

public:
	ASlashWeapon();
	
// Slash Weapon Interface
public:
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Equip(ASlashCharacter* Character);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachToHand(ACharacter* Character);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachToBack(ACharacter* Character);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ApplyEquippedSettings();	
	
protected:
	virtual void Interact_Implementation(AActor* InstigatorActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	TObjectPtr<USoundBase> EquipSound; 
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Weapon")
	FName BackSocketName;
};

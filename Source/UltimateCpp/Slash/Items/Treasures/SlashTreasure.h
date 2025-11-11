// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/SlashItem.h"
#include "SlashTreasure.generated.h"

UCLASS()
class ULTIMATECPP_API ASlashTreasure : public ASlashItem
{
	GENERATED_BODY()

public:
	ASlashTreasure();

protected:
	virtual void BeginPlay() override;

// Slash Interface:
public:	
	virtual void Interact_Implementation(AActor* Interactor) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Treasure")
	TObjectPtr<class UNiagaraComponent> GlowEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Treasure")
	int32 Gold;
	
};

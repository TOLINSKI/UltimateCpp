// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlashBreakableActor.h"
#include "BreakableWithItem.generated.h"

class ASlashItem;

UCLASS(Abstract)
class ULTIMATECPP_API ABreakableWithItem : public ASlashBreakableActor
{
	GENERATED_BODY()

public:
	ABreakableWithItem();

protected:
	virtual void BeginPlay() override;
	virtual void OnBreak(const FChaosBreakEvent& BreakEvent) override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Breakable|Item")
	TSubclassOf<ASlashItem> ItemClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Breakable|Item")
	TObjectPtr<USceneComponent> ItemPosition;

	UPROPERTY()
	TWeakObjectPtr<ASlashItem> Item;
};

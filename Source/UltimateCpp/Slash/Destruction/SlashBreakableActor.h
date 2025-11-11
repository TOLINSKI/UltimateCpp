// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/BC_DamageableInterface.h"
#include "SlashBreakableActor.generated.h"

class UGeometryCollectionComponent;
class USceneComponent;

UCLASS(Abstract)
class ULTIMATECPP_API ASlashBreakableActor : public AActor, public IBC_DamageableInterface
{
	GENERATED_BODY()

public:
	ASlashBreakableActor();

protected:
	virtual void BeginPlay() override;

// Slash Interface:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<USceneComponent> Root; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Destruction")
	float LifeSpan;
	
	UFUNCTION(BlueprintCallable, Category = "Destruction")
	virtual void OnBreak(const FChaosBreakEvent& BreakEvent);

private:
	bool bIsBroken;
};

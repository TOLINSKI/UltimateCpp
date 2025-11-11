// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BC_AttributeComponent.generated.h"


UCLASS(ClassGroup=(BaseCraft), meta=(BlueprintSpawnableComponent))
class BASECRAFT_API UBC_AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBC_AttributeComponent();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Attributes|Health")
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes|Health")
	float MaxHealth;

public:
	UFUNCTION(BlueprintCallable, Category="Attributes|State")
	FORCEINLINE bool IsAlive() { return Health > 0.f; }

	UFUNCTION(BlueprintPure, Category="Attributes|Health")
	FORCEINLINE float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, Category="Attributes|State")
	FORCEINLINE void SetHealth(float NewHealth) { Health = NewHealth; }

	/**
	 * Decreases then returns the remaining health.
	 * @param HealthToDecrease The amount of health to decrease
	 * @return The remaining health after decrease.
	 */
	UFUNCTION(BlueprintCallable, Category="Attributes|State")
	float DecreaseHealthBy(float HealthToDecrease);

	/**
	 * Increases then returns the new total health amount.
	 * @param HealthToIncrease The amount of health to add.
	 * @return Total health after increase.
	 */
	UFUNCTION(BlueprintCallable, Category="Attributes|State")
	float IncreaseHealthBy(float HealthToIncrease);
};

// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BC_HoverComponent.generated.h"

UENUM(BlueprintType)
enum class EBC_HoverFunc : uint8
{
	Cos,
	Sin
};

UCLASS(ClassGroup=(BaseCraft), DisplayName="Hovering Movement(Sinus)",meta=(BlueprintSpawnableComponent))
class BASECRAFT_API UBC_HoverComponent : public UActorComponent
{
	GENERATED_BODY()
// UE Interface
public:
	UBC_HoverComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;

// Hovering Interface
private:
	float RunningTime{ 0.0f };
	
	FVector StartLocation;

	void Hover(float DeltaTime);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Hover")
	bool bShouldHover;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Hover", meta = (EditCondition = "bShouldHover"))
	float Amplitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Hover", meta = (EditCondition = "bShouldHover"))
	float Frequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Hover", meta = (EditCondition = "bShouldHover"))
	FVector HoverDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Hover", meta = (EditCondition = "bShouldHover"))
	EBC_HoverFunc HoverFunc;
public:
	UFUNCTION(BlueprintCallable, Category = "BaseCraft|Hover")
	FORCEINLINE bool GetShouldHover() const { return bShouldHover; }

	UFUNCTION(BlueprintCallable, Category = "BaseCraft|Hover")
	FORCEINLINE void SetShouldHover(bool bHoverEnabled) { bShouldHover = bHoverEnabled; SetComponentTickEnabled(bHoverEnabled); }
};

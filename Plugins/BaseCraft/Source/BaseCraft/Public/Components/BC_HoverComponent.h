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

	virtual void SetActive(bool bNewActive, bool bReset = false) override;
protected:
	virtual void BeginPlay() override;

// Hovering Interface
private:
	float RunningTime{ 0.0f };
	FVector StartLocation;

	void Hover(float DeltaTime);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Hover")
	float Amplitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Hover")
	float Frequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Hover")
	FVector HoverDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft|Hover")
	EBC_HoverFunc HoverFunc;
};

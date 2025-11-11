// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BC_HitStopComponent.generated.h"


UCLASS(ClassGroup=(BaseCraft), meta=(BlueprintSpawnableComponent))
class BASECRAFT_API UBC_HitStopComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBC_HitStopComponent();

	UFUNCTION(BlueprintCallable, Category = "BaseCraft")
	void HitStop();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft", meta=(ClampMax="1.0", ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float TimeDilation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseCraft", meta=(ClampMin = "0.0", UIMin = "0.0"))
	float HitStopDuration;

	FTimerHandle HitStopTimer;

private:
	void HitStopTimeout() const;
};

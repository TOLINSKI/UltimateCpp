// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BC_HealthBarWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class BASECRAFT_API UBC_HealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="BaseCraft", meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

public:
	UFUNCTION(BlueprintCallable, Category="BaseCraft")
	void SetHealthPercent(float HealthPercent);

	UFUNCTION(BlueprintPure, Category="BaseCraft")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="BaseCraft|Visual")
	void SetBarColor(FLinearColor Color);
};

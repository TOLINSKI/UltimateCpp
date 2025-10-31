// Copyright Benski Game Works 2025, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "BC_InteractButtonComponent.generated.h"


UCLASS(ClassGroup=(BaseCraft), DisplayName="Interaction Button(Widget)", meta=(BlueprintSpawnableComponent))
class BASECRAFT_API UBC_InteractButtonComponent : public UWidgetComponent
{
	GENERATED_BODY()

// UE Interface:
public:
	UBC_InteractButtonComponent();

protected:
	virtual void BeginPlay() override;
	
// Base Craft Interface:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bStartWithWidgetCollapsed;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void ShowButton();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void HideButton();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void PressButton();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void SetButtonText(FText& Text);


};

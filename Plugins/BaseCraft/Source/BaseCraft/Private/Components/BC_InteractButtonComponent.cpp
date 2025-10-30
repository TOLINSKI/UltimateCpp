// Copyright Benski Game Works 2025, All rights reserved.


#include "Components/BC_InteractButtonComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BC_LogInteractionButton, All, All);

UBC_InteractButtonComponent::UBC_InteractButtonComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetUsingAbsoluteLocation(true);
	
	// Interaction Button Interface:
	bStartWithWidgetCollapsed = true;
	bAttachToOwnerLocation = false;
}

void UBC_InteractButtonComponent::BeginPlay()
{
	Super::BeginPlay();

	SetWorldLocation(GetOwner()->GetActorLocation());
	SetUsingAbsoluteLocation(!bAttachToOwnerLocation);

	if (bStartWithWidgetCollapsed && GetWidget())
	{
		GetWidget()->SetVisibility((ESlateVisibility::Collapsed));
	}
}

void UBC_InteractButtonComponent::ShowButton_Implementation()
{
	if (GetWidget())
	{
		GetWidget()->SetVisibility(ESlateVisibility::Visible);
	}
}

void UBC_InteractButtonComponent::HideButton_Implementation()
{
	if (GetWidget())
	{
		GetWidget()->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UBC_InteractButtonComponent::SetButtonText_Implementation(FText& Text)
{
	// Do in blueprints
}

void UBC_InteractButtonComponent::PressButton_Implementation()
{
	UE_LOG(BC_LogInteractionButton, Display, TEXT("PressButton() called."));
}
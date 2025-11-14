// Copyright Benski Game Works 2025, All rights reserved.


#include "UI/BC_HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UBC_HealthBarWidget::SetHealthPercent(float HealthPercent)
{
	HealthBar->SetPercent(HealthPercent);
}

float UBC_HealthBarWidget::GetHealthPercent() const
{
	return HealthBar->GetPercent();
}

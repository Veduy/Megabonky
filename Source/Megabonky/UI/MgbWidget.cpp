// Copyright is owned by Veduy.


#include "MgbWidget.h"

void UMgbWidget::HideWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UMgbWidget::ShowWidget()
{
	SetVisibility(ESlateVisibility::Visible);
}

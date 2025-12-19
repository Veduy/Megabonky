// Copyright is owned by Veduy.


#include "MgbWidgetInGame.h"
#include "Components/ProgressBar.h"
#include "MgbWidgetUpgradeEvent.h"

void UMgbWidgetInGame::NativeOnInitialized()
{
	if (XPBar)
	{
		XPBar->SetPercent(0);
	}

	if (UpgradeEvent)
	{
		UpgradeEvent->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMgbWidgetInGame::SetXPBarPercent(float NewPercent)
{
	if (XPBar)
	{
		XPBar->SetPercent(NewPercent);
	}
}

void UMgbWidgetInGame::ShowUpgradeEvent(bool bVisible)
{
	if (UpgradeEvent)
	{
		if (bVisible)
		{
			UpgradeEvent->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UpgradeEvent->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

// Copyright is owned by Veduy.


#include "InGame.h"
#include "Components/ProgressBar.h"
#include "ItemSelectWindow.h"
#include "ItemSelectButton.h"
#include "../../Core/MgbPlayerController.h"

#include "../../Util/NetworkLog.h"

void UInGame::NativeOnInitialized()
{
	if (XPBar)
	{
		XPBar->SetPercent(0);
	}

	if (ItemSelectWindow)
	{
		UItemSelectWindow* Widget = Cast<UItemSelectWindow>(ItemSelectWindow);
		Widget->HideWidget();
		
		Cast<UItemSelectButton>(Widget->ItemA)->OnItemSelected.AddDynamic(this, &UInGame::CompleteItemSelect);
		Cast<UItemSelectButton>(Widget->ItemB)->OnItemSelected.AddDynamic(this, &UInGame::CompleteItemSelect);
		Cast<UItemSelectButton>(Widget->ItemC)->OnItemSelected.AddDynamic(this, &UInGame::CompleteItemSelect);
	}
}

void UInGame::SetXPBarPercent(float NewPercent)
{
	if (XPBar)
	{
		XPBar->SetPercent(NewPercent);
	}
}

void UInGame::ShowItemSelectWindow()
{
	if (ItemSelectWindow)
	{
		ItemSelectWindow->ShowWidget();
	}
}

void UInGame::HideItemSelectWindow()
{
	if (ItemSelectWindow)
	{
		ItemSelectWindow->HideWidget();
	}
}

void UInGame::CompleteItemSelect()
{
	HideItemSelectWindow();

	AMgbPlayerController* PC = GetOwningPlayer<AMgbPlayerController>();
	if (PC)
	{
		NET_LOG("");
		PC->ServerResumeRequestCountIncrementAndCheck();
	}
}
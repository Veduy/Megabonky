// Copyright is owned by Veduy.


#include "InGame.h"
#include "Components/ProgressBar.h"
#include "ItemSelectWindow.h"
#include "ItemSelectButton.h"

//임시추가
#include "Kismet/GameplayStatics.h"

void UInGame::NativeOnInitialized()
{
	if (XPBar)
	{
		XPBar->SetPercent(0);
	}

	if (ItemSelectWindow)
	{
		ItemSelectWindow->HideWidget();

		ItemSelectWindow->ItemA->OnItemSelected.AddDynamic(this, &UInGame::HideItemSelectWindow);
		ItemSelectWindow->ItemB->OnItemSelected.AddDynamic(this, &UInGame::HideItemSelectWindow);
		ItemSelectWindow->ItemC->OnItemSelected.AddDynamic(this, &UInGame::HideItemSelectWindow);
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

		//임시로 게임 진행시킴.
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

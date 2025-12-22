// Copyright is owned by Veduy.


#include "InGame.h"
#include "Components/ProgressBar.h"
#include "ItemSelectWindow.h"
#include "ItemSelectButton.h"
#include "../../Core/MgbPlayerController.h"


void UInGame::NativeOnInitialized()
{
	if (XPBar)
	{
		XPBar->SetPercent(0);
	}

	if (ItemSelectWindow)
	{
		ItemSelectWindow->HideWidget();

		ItemSelectWindow->ItemA->OnItemSelected.AddDynamic(this, &UInGame::CompleteItemSelect);
		ItemSelectWindow->ItemB->OnItemSelected.AddDynamic(this, &UInGame::CompleteItemSelect);
		ItemSelectWindow->ItemC->OnItemSelected.AddDynamic(this, &UInGame::CompleteItemSelect);
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
	// PlayerController 에서 Server RPC 호출.
	// Server의 GameState의 GameResumeRequestCount 변수 값 증가 후 
	// 현재 게임중인 플레이어 수와 일치되는지 확인 한 다음에, Server에 SetGamepause(false) 호출.
	AMgbPlayerController* PC = GetOwningLocalPlayer<AMgbPlayerController>();
	if (PC)
	{
		PC->ServerResumeRequestCountIncrementAndCheck();
	}
}
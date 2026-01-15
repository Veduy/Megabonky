// Copyright is owned by Veduy.


#include "ItemSelectButton.h"
#include "Components/Button.h"
#include "../../Core/MgbPlayerController.h"
#include "../../Util/NetworkLog.h"

void UItemSelectButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UItemSelectButton::HandleButtonClicked);
	}
}

void UItemSelectButton::HandleButtonClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (AMgbPlayerController* MgbPC = Cast<AMgbPlayerController>(PC))
	{
		MgbPC->ServerApplyWeaponUpgradeEffect(UpgradeItemName, Upgrades);
	}

	OnItemSelected.Broadcast();
}

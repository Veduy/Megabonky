// Copyright is owned by Veduy.


#include "ItemSelectButton.h"
#include "Components/Button.h"

void UItemSelectButton::NativeOnInitialized()
{
	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UItemSelectButton::HandleButtonClicked);
	}
}

void UItemSelectButton::HandleButtonClicked()
{
	OnItemSelected.Broadcast();
}

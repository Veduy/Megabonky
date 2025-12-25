// Copyright is owned by Veduy.


#include "ItemSelectButton.h"
#include "Components/Button.h"

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
	NET_LOG("");
	OnItemSelected.Broadcast();
}

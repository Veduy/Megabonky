// Copyright is owned by Veduy.


#include "Title.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "../../MgbSubsystem.h"

void UTitle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (DuoBtn)
	{
		DuoBtn->OnClicked.AddDynamic(this, &UTitle::HandleDuoBtnClicked);
	}
}

void UTitle::HandleDuoBtnClicked()
{
	UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
	if (GI)
	{
		UMgbSubsystem* MgbSubSystem = GI->GetSubsystem<UMgbSubsystem>();

		MgbSubSystem->Login();
	}
}

// Copyright is owned by Veduy.


#include "Title.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "../../MgbSubsystem.h"

void UTitle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (PlayBtn)
	{
		PlayBtn->OnClicked.AddDynamic(this, &UTitle::HandlePlayBtnClicked);
	}

	if (DuoBtn)
	{
		DuoBtn->OnClicked.AddDynamic(this, &UTitle::HandleDuoBtnClicked);
	}
}

void UTitle::HandlePlayBtnClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Game"));
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

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
}

void UTitle::HandlePlayBtnClicked()
{
	//GetWorld()->ServerTravel(TEXT("/Game/Maps/GameMap?listen"));
}

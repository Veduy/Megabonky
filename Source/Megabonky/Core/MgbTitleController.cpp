// Copyright is owned by Veduy.

#include "MgbTitleController.h"
#include "Kismet/GameplayStatics.h"

void AMgbTitleController::ServerStartGame_Implementation()
{
	if (!HasAuthority())
		return;

	// Transition to..
	FString MapName = TEXT("Game");

	GetWorld()->ServerTravel(FString::Printf(TEXT("/Game/Maps/%s"), *MapName));
}
	
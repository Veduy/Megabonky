// Copyright is owned by Veduy.


#include "MgbTitleController.h"
#include "Kismet/GameplayStatics.h"

void AMgbTitleController::ServerStartGame_Implementation()
{
	if (!HasAuthority())
		return;

	// Transition to the main game level
	FString GameMapName = TEXT("Game");

	GetWorld()->ServerTravel(FString::Printf(TEXT("/Game/Maps/%s"), *GameMapName));
}

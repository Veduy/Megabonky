// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MgbTitleController.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKY_API AMgbTitleController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable, Category = "Megabonky|Login")
	void ServerStartGame();
	void ServerStartGame_Implementation();
};

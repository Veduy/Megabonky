// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MgbGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class MEGABONKY_API AMgbGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMgbGameModeBase();

	virtual void BeginPlay() override;
	virtual void StartPlay() override;

public:


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 bSpawnEnemy : 1 = false;
};

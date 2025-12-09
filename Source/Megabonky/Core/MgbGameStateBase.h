// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MgbGameStateBase.generated.h"

class AMgbEnemyCharacter;

/**
 * 
 */
UCLASS()
class MEGABONKY_API AMgbGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMgbGameStateBase();

	virtual void BeginPlay() override;

public:
	void InitSpawnEnemyTimer();
	void SpawnEnemy();

public:
	UPROPERTY()
	TArray<TSubclassOf<AMgbEnemyCharacter>> EnemyClasses;

	UPROPERTY()
	int32 MaxEnemyCount = 100;
};

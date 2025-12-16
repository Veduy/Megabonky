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
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

public:
	void InitSpawnEnemyTimer();

	void SpawnEnemy();

public:
	UFUNCTION()
	void OnRep_XP();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<TSubclassOf<AMgbEnemyCharacter>> EnemyClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data", ReplicatedUsing = OnRep_XP)
	float XP = 0;
};

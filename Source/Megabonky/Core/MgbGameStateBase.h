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
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerAddXP(float InValue);
	void ServerAddXP_Implementation(float InValue);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastUpdateUI_XP(float InPercent);
	void MulticastUpdateUI_XP_Implementation(float InPercent);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastSetPauseGame(bool bPause);
	void MulticastSetPauseGame_Implementation(bool bPause);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastShowItemSelectWindow();
	void MulticastShowItemSelectWindow_Implementation();

public:
	void InitSpawnEnemyTimer();
	void SpawnEnemy();
	void HandleResumeRequest();
	uint32 GetCurrentPlayerCount();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TArray<TSubclassOf<AMgbEnemyCharacter>> EnemyClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	int32 MaxEnemyCount = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	float SpawnIntervalTime = 5.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	float SpawnRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	int32 MonstersPerSpawn = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CurrentLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	float RequiredXP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	float CurrentXP = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ResumeRequestCount = 0;
};

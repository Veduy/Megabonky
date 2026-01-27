// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MgbGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChanged, int32, PassedTime);

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
	
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnTimeChanged OnTimeChanged;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<TSubclassOf<AMgbEnemyCharacter>> EnemyClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	float GameStartTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	int32 GameTimeSec = 600;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	int32 PassedTimeSec = 0;

	float ElapsedTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ResumeRequestCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	int32 MaxEnemyCount = 1000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CurrentEnemyCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	float SpawnIntervalTime = 5.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	float SpawnRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	int32 EnemyPerSpawn = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	float RequiredXP = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	float CurrentXP = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	int32 CurrentLevel = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	int32 Gold = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	int64 TotalKill = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	int32 NextBoxGold = 30;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> DT_Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> DT_WeaponUpgradeBonus;
};

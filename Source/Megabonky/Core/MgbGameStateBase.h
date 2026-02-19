// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MgbGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChanged, int32, PassedTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastHandleGameOver();
	void MulticastHandleGameOver_Implementation();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerHandleGameOver();
	void ServerHandleGameOver_Implementation();
	
public:
	UFUNCTION()
	void InitSpawnEnemyTimer();

	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION(BlueprintCallable)
	void ActivateSpawnMultiplierBurst(int32 InValue);

	UFUNCTION()
	void HandleGameOver();

	UFUNCTION()
	void HandleResumeRequest();

	// 무기별 킬 수 기록
	void RecordWeaponKill(FName WeaponName);

	uint32 GetCurrentPlayerCount();

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnTimeChanged OnTimeChanged;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnGameOver OnGameOver;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTimerHandle SpawnTimerHandle;

	FTimerHandle SpawnMultiplierResetTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<TSubclassOf<AMgbEnemyCharacter>> EnemyClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	uint8 bGameOver : 1 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	float GameStartTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	int32 GameTimeSec = 600;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	int32 PassedTimeSec = 0;

	float ElapsedTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ResumeRequestCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 MaxEnemyCount = 1000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CurrentEnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float SpawnIntervalTime = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float SpawnRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 EnemySpawnMultiplier = 1;

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

	// 무기별 킬 통계 조회
	const TMap<FName, int64>& GetWeaponKillRecord() const { return WeaponKillRecord; }

private:
	// 현재 세션의 무기별 킬 수 (메모리에만 저장, 게임 종료 시 파일로 저장)
	TMap<FName, int64> WeaponKillRecord;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> DT_Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> DT_WeaponUpgradeBonus;
};

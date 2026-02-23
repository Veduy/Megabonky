// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyMovementOptimizer.generated.h"

class AMgbEnemyCharacter;
class AMgbPlayerCharacter;

/**
 * 플레이어와의 거리에 따라 적 캐릭터의 틱/네트워크 업데이트 빈도를 조절하는 월드 서브시스템.
 * 각 적이 개별적으로 거리 체크하는 대신, 중앙에서 일괄 관리하여 오버헤드를 최소화합니다.
 */
UCLASS()
class MEGABONKY_API UEnemyMovementOptimizer : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	// 서버에서만 동작
	virtual bool IsTickable() const override;

	void RegisterEnemy(AMgbEnemyCharacter* Enemy);
	void UnregisterEnemy(AMgbEnemyCharacter* Enemy);

	void RegisterPlayer(AMgbPlayerCharacter* Player);
	void UnregisterPlayer(AMgbPlayerCharacter* Player);

private:
	void UpdateEnemyTickRates();

	UPROPERTY()
	TArray<TWeakObjectPtr<AMgbEnemyCharacter>> RegisteredEnemies;

	UPROPERTY()
	TArray<TWeakObjectPtr<AMgbPlayerCharacter>> CachedPlayers;

	// 업데이트 주기 제어 (매 프레임이 아닌 일정 간격으로 순회)
	float TimeSinceLastUpdate = 0.f;
	float UpdateInterval = 0.3f;

	// 한 프레임에 처리할 적 수 (분산 처리용)
	int32 BatchStartIndex = 0;
	int32 BatchSize = 50;

	// 거리 임계값 (제곱 거리로 비교하여 sqrt 비용 제거)
	float NearDistSq = 1500.f * 1500.f;   // 2,250,000
	float MidDistSq  = 3000.f * 3000.f;   // 9,000,000
};

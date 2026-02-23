// Copyright is owned by Veduy.

#include "EnemyMovementOptimizer.h"

#include "Characters/MgbEnemyCharacter.h"
#include "Characters/MgbPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyMovementOptimizer::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	RegisteredEnemies.Reserve(256);
}

void UEnemyMovementOptimizer::Deinitialize()
{
	RegisteredEnemies.Empty();
	CachedPlayers.Empty();
	Super::Deinitialize();
}

TStatId UEnemyMovementOptimizer::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UEnemyMovementOptimizer, STATGROUP_Tickables);
}

bool UEnemyMovementOptimizer::IsTickable() const
{
	UWorld* World = GetWorld();
	return World && World->GetNetMode() != NM_Client;
}

void UEnemyMovementOptimizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastUpdate += DeltaTime;
	if (TimeSinceLastUpdate < UpdateInterval)
	{
		return;
	}
	TimeSinceLastUpdate = 0.f;

	UpdateEnemyTickRates();
}

void UEnemyMovementOptimizer::RegisterEnemy(AMgbEnemyCharacter* Enemy)
{
	if (Enemy)
	{
		RegisteredEnemies.Add(Enemy);
	}
}

void UEnemyMovementOptimizer::UnregisterEnemy(AMgbEnemyCharacter* Enemy)
{
	RegisteredEnemies.RemoveSwap(Enemy);
}

void UEnemyMovementOptimizer::RegisterPlayer(AMgbPlayerCharacter* Player)
{
	if (Player)
	{
		CachedPlayers.AddUnique(Player);
	}
}

void UEnemyMovementOptimizer::UnregisterPlayer(AMgbPlayerCharacter* Player)
{
	CachedPlayers.RemoveSwap(Player);
}

void UEnemyMovementOptimizer::UpdateEnemyTickRates()
{
	// 무효화된 약참조 정리
	RegisteredEnemies.RemoveAll([](const TWeakObjectPtr<AMgbEnemyCharacter>& Ptr)
	{
		return !Ptr.IsValid();
	});
	CachedPlayers.RemoveAll([](const TWeakObjectPtr<AMgbPlayerCharacter>& Ptr)
	{
		return !Ptr.IsValid();
	});

	const int32 TotalCount = RegisteredEnemies.Num();
	if (TotalCount == 0 || CachedPlayers.Num() == 0)
	{
		return;
	}

	// 플레이어 위치 캐싱 (매 업데이트마다 한번만)
	TArray<FVector, TInlineAllocator<4>> PlayerLocations;
	for (const auto& PlayerPtr : CachedPlayers)
	{
		if (PlayerPtr.IsValid())
		{
			PlayerLocations.Add(PlayerPtr->GetActorLocation());
		}
	}

	if (PlayerLocations.Num() == 0)
	{
		return;
	}

	// 배치 범위 계산 (한 프레임에 BatchSize만큼만 처리)
	if (BatchStartIndex >= TotalCount)
	{
		BatchStartIndex = 0;
	}
	const int32 EndIndex = FMath::Min(BatchStartIndex + BatchSize, TotalCount);

	for (int32 i = BatchStartIndex; i < EndIndex; ++i)
	{
		AMgbEnemyCharacter* Enemy = RegisteredEnemies[i].Get();
		if (!Enemy)
		{
			continue;
		}

		// 가장 가까운 플레이어와의 거리(제곱) 계산
		const FVector EnemyLoc = Enemy->GetActorLocation();
		float MinDistSq = MAX_FLT;
		for (const FVector& PlayerLoc : PlayerLocations)
		{
			const float DistSq = FVector::DistSquared(EnemyLoc, PlayerLoc);
			if (DistSq < MinDistSq)
			{
				MinDistSq = DistSq;
			}
		}

		UCharacterMovementComponent* CMC = Enemy->GetCharacterMovement();
		if (!CMC)
		{
			continue;
		}

		// 거리 구간별 틱/네트워크 설정
		if (MinDistSq < NearDistSq)
		{
			// Near: 풀 틱
			CMC->SetComponentTickInterval(0.f);
			Enemy->SetActorTickInterval(0.f);
			Enemy->SetNetUpdateFrequency(30.f);
		}
		else if (MinDistSq < MidDistSq)
		{
			// Mid: 줄인 틱
			CMC->SetComponentTickInterval(0.05f);
			Enemy->SetActorTickInterval(0.05f);
			Enemy->SetNetUpdateFrequency(15.f);
		}
		else
		{
			// Far: 최소 틱
			CMC->SetComponentTickInterval(0.1f);
			Enemy->SetActorTickInterval(0.1f);
			Enemy->SetNetUpdateFrequency(10.f);
		}
	}

	BatchStartIndex = EndIndex;
}

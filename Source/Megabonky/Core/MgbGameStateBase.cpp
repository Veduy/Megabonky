// Copyright is owned by Veduy.


#include "MgbGameStateBase.h"

#include "Characters/MgbEnemyCharacter.h"
#include "../Util/NetworkLog.h"

AMgbGameStateBase::AMgbGameStateBase()
{
}

void AMgbGameStateBase::BeginPlay()
{
	Super::BeginPlay();

}

void AMgbGameStateBase::InitSpawnEnemyTimer()
{
	FTimerHandle SpawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		[this]()
		{
			SpawnEnemy();   
		},
		5.0f,
		true,
		3.f
	);
}

void AMgbGameStateBase::SpawnEnemy()
{
	// Only Server
	if (!HasAuthority())
	{
		return;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (Iterator->Get())
		{
			NET_LOG(FString::Printf(TEXT("Controller : %s"), *Iterator->Get()->GetName()));
			APawn* Player = Iterator->Get()->GetPawn();
		
			// 일단은 0,0 (캐릭터의 좌표)좌표를 기준으로 원형태의 랜덤 방향을 구함.
			float y = sinf(rand());
			float x = cosf(rand());
			FVector2D Dir = FVector2D(x, y);
			NET_LOG(FString::Printf(TEXT("Large Spawn Direction : x:%f, y:%f"), x, y));

			// 일정 Min~Max범위의 값을 곱함 -> 캐릭터 근처 에서 스폰될 거리 지정.
			FVector2D Location = Dir * FMath::FRandRange(500.f, 1000.f);

			// 스폰할 지점에서 또 작은 원을 기준으로 스폰할 마리수에 해당하는, 진짜 스폰 지점을 뽑아내서 그 지점에 스폰.
			for (int i = 0; i < 5; ++i)
			{
				FVector2D SmallDir = FVector2D(cosf(rand()), sinf(rand()));
				FVector2D SmallLocation = SmallDir * FMath::FRandRange(100.f, 200.f);

				FVector2D RealSpawn2D = Location + SmallLocation;

				// 일단은 Z = 0
				// 지형에 따라 높이는 라인트레이싱으로 계산 하도록 수정.
				FVector RealSpawnLocation = FVector(RealSpawn2D.X, RealSpawn2D.Y, 0);

				// 임시로 플레이어 위치 더해봄.
				RealSpawnLocation = RealSpawnLocation + Player->GetActorLocation();
				GetWorld()->SpawnActor<AActor>(EnemyClasses[0], RealSpawnLocation, FRotator::ZeroRotator);
			}
		}
	}




}
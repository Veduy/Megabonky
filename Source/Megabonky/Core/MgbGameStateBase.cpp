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
	// 게임 시작시 타이머로 일정 시간마다 스폰하도록 로직.
	FTimerHandle SpawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		[this]()
		{
			SpawnEnemy();   // UFUNCTION() 호출 가능
		},
		5.0f,
		true,
		3.f
	);
}

void AMgbGameStateBase::SpawnEnemy()
{
	if (!HasAuthority())
	{
		return;
	}

	// 멀티 환경이라면 각각의 플레이어 근처에 스폰되도록.
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (Iterator->Get())
		{
		}
	}

	//GetWorld()->SpawnActor(EnemyClasses[0], );
}
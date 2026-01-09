// Copyright is owned by Veduy.


#include "MgbGameModeBase.h"

#include "MgbGameStateBase.h"
#include "../Util/NetworkLog.h"

AMgbGameModeBase::AMgbGameModeBase()
{

}

void AMgbGameModeBase::BeginPlay()
{
	Super::BeginPlay();


}

void AMgbGameModeBase::StartPlay()
{
	Super::StartPlay();

	// 게임이 시작됬을때 행해져야하는것들;
	//1.EnemySpawn
	//2.스테이지 Timer 동작
	AMgbGameStateBase* GS = GetGameState<AMgbGameStateBase>();
	if (GS)
	{
		if (bSpawnEnemy)
		{
			GS->InitSpawnEnemyTimer();
		}

		
	}
}
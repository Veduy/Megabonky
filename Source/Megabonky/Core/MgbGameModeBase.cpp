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

	AMgbGameStateBase* GS = GetGameState<AMgbGameStateBase>();
	if (GS)
	{
		if (bSpawnEnemy)
		{
			GS->InitSpawnEnemyTimer();
		}
	}
}

void AMgbGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::EndPlay(EndPlayReason);
}
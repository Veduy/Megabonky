// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MgbSubsystem.generated.h"

/**
 * 타이틀과 인게임 간 데이터 유지
 */
UCLASS()
class MEGABONKY_API UMgbSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

public:


};

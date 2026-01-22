// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "MgbSubsystem.generated.h"

class FHttpModule;

/**
 * 타이틀과 인게임 간 데이터 유지
 */
UCLASS()
class MEGABONKY_API UMgbSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

public:
	void RequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully);

	void Login();

	FHttpModule* HttpModule;

};

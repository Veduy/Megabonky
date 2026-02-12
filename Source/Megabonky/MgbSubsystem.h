// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Core/Data/MgbSaveGame.h"
#include "MgbSubsystem.generated.h"

class FHttpModule;

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

	// Save/Load
	void SaveGameSession(const FGameSessionRecord& SessionRecord);
	UMgbSaveGame* LoadGameData();

	// Query records
	const UMgbSaveGame* GetCurrentSaveData() const { return CurrentSaveData; }

public:
	FHttpModule* HttpModule;

private:
	UPROPERTY()
	TObjectPtr<UMgbSaveGame> CurrentSaveData;

	static const FString SaveSlotName;  // "MegabonkyPlayerData"
	static const int32 SaveUserIndex;   // 0

	void UpdateBestRecords(UMgbSaveGame* InOutSaveData, const FGameSessionRecord& NewRecord);
	void UpdateTotalWeaponKills(UMgbSaveGame* InOutSaveData, const TArray<FWeaponKillRecord>& WeaponKills);
};

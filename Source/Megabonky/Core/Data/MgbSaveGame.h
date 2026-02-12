// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MgbSaveGame.generated.h"

/**
 * Weapon kill statistics
 */
USTRUCT(BlueprintType)
struct FWeaponKillRecord
{
	GENERATED_BODY()

public:
	FWeaponKillRecord() : WeaponName(NAME_None), KillCount(0) {}
	FWeaponKillRecord(FName InName, int64 InCount) : WeaponName(InName), KillCount(InCount) {}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName WeaponName;                // Weapon name (e.g., "Dagger", "Bananarang")

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int64 KillCount;                 // Number of enemies killed by this weapon
};

/**
 * Game session record
 */
USTRUCT(BlueprintType)
struct FGameSessionRecord
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int64 TotalKills = 0;                        // Total kills

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 GoldEarned = 0;                        // Gold earned

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FWeaponKillRecord> WeaponKills;       // Weapon kill statistics
};

/**
 * Game progress save data
 */
UCLASS()
class MEGABONKY_API UMgbSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UMgbSaveGame();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameSessionRecord BestRecord_Kills;        // Best kill record

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TotalGamesPlayed = 0;                 // Total games played

	// Cumulative statistics (all games combined)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, int64> TotalWeaponKills;        // Total weapon kills (cumulative)
};

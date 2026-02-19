// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WeaponInfo.generated.h" 

class AMgbWeapon;
class UTexture2D;

USTRUCT(BlueprintType)
struct FMgbWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMgbWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;
};

UENUM(BlueprintType)
enum class EWeaponUpgradeStat : uint8
{
	Damage,
	CritChance,
	CritDamage,
	ProjectileCount,
	ProjectileSpeed,
	ProjectileBounce,
	Size,
	Knockback,
	Duration
};

USTRUCT(BlueprintType)
struct FWeaponUpgradeOption
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponUpgradeStat StatType = EWeaponUpgradeStat::Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float IncreaseValue = 0.f; 
};

USTRUCT(BlueprintType)
struct FMgbWeaponUpgradeBonus : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FWeaponUpgradeOption> UpgradeOptions;
};

USTRUCT(BlueprintType)
struct FUpgradeSlotInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FName WeaponName;

	UPROPERTY(BlueprintReadOnly)
	TArray<FWeaponUpgradeOption> Options;
};
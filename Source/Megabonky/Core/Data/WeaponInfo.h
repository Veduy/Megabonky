// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInfo.generated.h" 

class UTexture2D;

USTRUCT(BlueprintType)
struct FMgbWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;
};
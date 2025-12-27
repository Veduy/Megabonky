// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbWidget.h"
#include "DamageText.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class MEGABONKY_API UDamageText : public UMgbWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetDamageText(float InValue);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UTextBlock> Value;
};

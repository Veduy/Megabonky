// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "MgbWidget.h"
#include "MgbWidgetInGame.generated.h"

class UProgressBar;
class UMgbWidgetUpgradeEvent;
/**
 * 
 */
UCLASS()
class MEGABONKY_API UMgbWidgetInGame : public UMgbWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
public:
	UFUNCTION()
	void SetXPBarPercent(float NewPercent);

	UFUNCTION()
	void ShowUpgradeEvent(bool bVisible);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (WidgetBind))
	TObjectPtr<UProgressBar> XPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (WidgetBind))
	TObjectPtr<UMgbWidgetUpgradeEvent> UpgradeEvent;

};
 
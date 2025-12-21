// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbWidget.h"
#include "InGame.generated.h"

class UProgressBar;
class UItemSelectWindow;
/**
 * 
 */
UCLASS()
class MEGABONKY_API UInGame : public UMgbWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
public:
	UFUNCTION()
	void SetXPBarPercent(float NewPercent);

	UFUNCTION()
	void ShowItemSelectWindow();

	UFUNCTION()
	void HideItemSelectWindow();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UProgressBar> XPBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UItemSelectWindow> ItemSelectWindow;
};
 
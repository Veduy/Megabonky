// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbWidget.h"
#include "../../Core/Data/WeaponInfo.h"
#include "InGame.generated.h"

class UProgressBar;
class UTextBlock;
class UTexture2D;

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

	UFUNCTION()
	void CompleteItemSelect();

	UFUNCTION(BlueprintImplementableEvent)
	void SetItemUpgradeSlot(const int32 SlotNum, const FName ItemName, const TArray<FWeaponUpgradeOption>& UpgradeStat);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UProgressBar> XPBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UMgbWidget> ItemSelectWindow;

	/// <summary>
	/// StateInfoBox
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UTextBlock> PassedTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UTextBlock> Silver;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UTextBlock> Gold;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UTextBlock> TotalKill;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UTextBlock> RemainingTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UTextBlock> NextBoxGold;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UTextBlock> Level;

};
 
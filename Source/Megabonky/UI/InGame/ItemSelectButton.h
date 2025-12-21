// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbWidget.h"
#include "ItemSelectButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemSelected);

class UImage;
class UButton;
class UTextBlock;
class URichTextBlock;
/**
 * 
 */
UCLASS()
class MEGABONKY_API UItemSelectButton : public UMgbWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void HandleButtonClicked();

public:
	UPROPERTY()
	FOnItemSelected OnItemSelected;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UImage> Background;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<URichTextBlock> Tier;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UTextBlock> Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<URichTextBlock> Description;
};

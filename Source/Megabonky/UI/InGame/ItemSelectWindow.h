// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbWidget.h"
#include "ItemSelectWindow.generated.h"

class UItemSelectButton;
/**
 * 
 */
UCLASS()
class MEGABONKY_API UItemSelectWindow : public UMgbWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UItemSelectButton> ItemA;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UItemSelectButton> ItemB;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UItemSelectButton> ItemC;
}; 
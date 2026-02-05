// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbWidget.h"
#include "Title.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class MEGABONKY_API UTitle : public UMgbWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void HandlePlayBtnClicked();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (BindWidget))
	TObjectPtr<UButton> PlayBtn;
};

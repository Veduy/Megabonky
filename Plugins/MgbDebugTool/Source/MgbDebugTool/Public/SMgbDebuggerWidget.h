

#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SMgbDebuggerWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SMgbDebuggerWidget) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    FReply OnRefreshClicked();
};


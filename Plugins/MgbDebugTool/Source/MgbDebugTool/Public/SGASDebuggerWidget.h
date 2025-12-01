

#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SGASDebuggerWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SGASDebuggerWidget) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    FReply OnRefreshClicked();
};



#include "SGASDebuggerWidget.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"

void SGASDebuggerWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot().AutoHeight()
			[	
				SNew(SButton)
					.Text(FText::FromString("Refresh"))
					.OnClicked(this, &SGASDebuggerWidget::OnRefreshClicked)
			]
		];
}

FReply SGASDebuggerWidget::OnRefreshClicked()
{
	return FReply::Handled();
}




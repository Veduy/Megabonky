
#include "SMgbDebuggerWidget.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "PropertyCustomizationHelpers.h"

void SMgbDebuggerWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SVerticalBox)

				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(SButton)
						.Text(FText::FromString("Refresh"))
						.OnClicked(this, &SMgbDebuggerWidget::OnRefreshClicked)
				]

				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(STextBlock)
						.Text(FText::FromString("Select Test"))
				]

				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(SObjectPropertyEntryBox)
						.AllowedClass(AActor::StaticClass())
						//.OnObjectChanged_Lambda(this, []() {})
						.AllowClear(true)
				]
		];
}

FReply SMgbDebuggerWidget::OnRefreshClicked()
{
	return FReply::Handled();
}




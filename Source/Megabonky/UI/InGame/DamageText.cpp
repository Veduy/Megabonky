// Copyright is owned by Veduy.


#include "DamageText.h"
#include "Components/TextBlock.h"

void UDamageText::SetDamageText(float InValue)
{
	FText NewText = FText::AsNumber(FMath::TruncToInt(InValue));

	Value->SetText(NewText);
}
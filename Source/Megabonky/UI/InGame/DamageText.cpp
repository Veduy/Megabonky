// Copyright is owned by Veduy.


#include "DamageText.h"
#include "Components/TextBlock.h"

void UDamageText::SetDamageText(float InValue)
{
	//FText NewText = FText::FromString(FString::Printf(TEXT("%f"), InValue));
	FText NewText = FText::AsNumber(InValue);

	Value->SetText(NewText);
}
// Copyright is owned by Veduy.


#include "MgbEffectExecutionCalculation.h"

#include "AttributeSet/CharacterAttributeSet.h"
#include "AttributeSet/PlayerAttributeSet.h"
#include "AttributeSet/WeaponAttributeSet.h"

#include "../../Util/NetworkLog.h"

UMgbEffectExecutionCalculation::UMgbEffectExecutionCalculation(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, Damage, Source, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Health, Target, false);

	// Execution calculation uses these captures
	RelevantAttributesToCapture.Add(DamageDef);
	RelevantAttributesToCapture.Add(HealthDef);
}

void UMgbEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	NET_LOG("");

	//FGameplayEffectAttributeCaptureSpecContainer SpecContainer = ExecutionParams.GetOwningSpec().CapturedRelevantAttributes;

	//bool FGameplayEffectCustomExecutionParameters::AttemptCalculateCapturedAttributeMagnitude(const FGameplayEffectAttributeCaptureDefinition & InCaptureDef, const FAggregatorEvaluateParameters & InEvalParams, OUT float& OutMagnitude) const
	
	FAggregatorEvaluateParameters EvaluatedParams;

	EvaluatedParams.SourceTags = ExecutionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
	EvaluatedParams.TargetTags = ExecutionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

	float Damage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageDef, EvaluatedParams, Damage);
	
	NET_LOG(FString::Printf(TEXT("Source Damage Attribute: %f"), Damage));

	UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), Damage);

	return OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(UCharacterAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -Damage));

}

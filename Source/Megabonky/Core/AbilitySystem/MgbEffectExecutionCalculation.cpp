// Copyright is owned by Veduy.


#include "MgbEffectExecutionCalculation.h"

#include "AttributeSet/CharacterAttributeSet.h"
#include "AttributeSet/PlayerAttributeSet.h"
#include "AttributeSet/WeaponAttributeSet.h"

#include "../../Util/NetworkLog.h"

UMgbEffectExecutionCalculation::UMgbEffectExecutionCalculation(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Onwership
	// GameplayEffect

	// Source(Weapon) = WeaponAttributeset
	// ->Damage
	// ->CritChance
	// ->CritDamage

	DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, Damage, Source, false);

	// PlayerCharacter->WeaponAttributeset
	// ->Damage
	// ->CritChance
	// ->CritDamage
	// ->DamageToElite
	
	
	// Target -> EnemyCharacter
	DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Health, Target, false);

	// Execution calculation uses these captures
	RelevantAttributesToCapture.Add(DamageDef);
	RelevantAttributesToCapture.Add(HealthDef);
}

void UMgbEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{	
	FAggregatorEvaluateParameters EvaluatedParams;

	EvaluatedParams.SourceTags = ExecutionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
	EvaluatedParams.TargetTags = ExecutionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

	float Damage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageDef, EvaluatedParams, Damage);

	return OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(UCharacterAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -Damage));

}
// Copyright is owned by Veduy.


#include "MgbEffectExecutionCalculation.h"

#include "AttributeSet/CharacterAttributeSet.h"
#include "AttributeSet/PlayerAttributeSet.h"
#include "AttributeSet/WeaponAttributeSet.h"

#include "../Characters/MgbPlayerCharacter.h"
#include "../MgbWeapon.h"

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

	// PlayerCharacter->WeaponAttributeset
	// ->Damage
	// ->CritChance
	// ->CritDamage
	// ->DamageToElite

	// Source가 누구지.

	DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, Damage, Source, false);

	// Target -> EnemyCharacter
	DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Health, Target, false);

	// Execution calculation uses these captures
	RelevantAttributesToCapture.Add(DamageDef);
	RelevantAttributesToCapture.Add(HealthDef);
}

void UMgbEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{	
	// SourceObj(Weapon) Player의 Attribute값 정보 가져와서 계산.
	UObject* SourceObj = ExecutionParams.GetOwningSpec().GetEffectContext().GetSourceObject();
	//AMgbPlayerCharacter* PlayerCharacter = Cast<AMgbPlayerCharacter>(SourceObj);
	AMgbWeapon* Weapon = Cast<AMgbWeapon>(SourceObj);
	AMgbPlayerCharacter* PlayerCharacter = Cast<AMgbPlayerCharacter>(Weapon->GetOwner());

	FAggregatorEvaluateParameters EvaluatedParams;

	EvaluatedParams.SourceTags = ExecutionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
	EvaluatedParams.TargetTags = ExecutionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

	float PlayerDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageDef, EvaluatedParams, PlayerDamage);

	float TotalDamage = 0.f;

	return OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(UCharacterAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -TotalDamage));

}
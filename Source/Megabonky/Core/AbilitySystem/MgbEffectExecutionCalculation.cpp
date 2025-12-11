// Copyright is owned by Veduy.


#include "MgbEffectExecutionCalculation.h"

#include "../AbilitySystem/MgbAbilitySystemComponent.h"
#include "AttributeSet/CharacterAttributeSet.h"
#include "AttributeSet/PlayerAttributeSet.h"
#include "AttributeSet/WeaponAttributeSet.h"

#include "../Characters/MgbPlayerCharacter.h"
#include "../MgbWeapon.h"

#include "../../Util/NetworkLog.h"

UMgbEffectExecutionCalculation::UMgbEffectExecutionCalculation(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Source (Weapon)
	DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, Damage, Source, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, CritChance, Source, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, CritDamage, Source, false);
	
	// Target (EnemyCharacter)
	DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Health, Target, false);

	RelevantAttributesToCapture.Add(DamageDef);
	RelevantAttributesToCapture.Add(CritChanceDef);
	RelevantAttributesToCapture.Add(CritDamageDef);

	RelevantAttributesToCapture.Add(HealthDef);
}

void UMgbEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{	
	// SourceObj(Weapon)->Owner(Player)의 Attribute값 필요.
	UObject* SourceObj = ExecutionParams.GetOwningSpec().GetEffectContext().GetSourceObject();
	AMgbWeapon* Weapon = Cast<AMgbWeapon>(SourceObj);
	AMgbPlayerCharacter* PlayerCharacter = Cast<AMgbPlayerCharacter>(Weapon->GetOwner());

	FAggregatorEvaluateParameters EvaluatedParams;
	EvaluatedParams.SourceTags = ExecutionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
	EvaluatedParams.TargetTags = ExecutionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

	float WeaponDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageDef, EvaluatedParams, WeaponDamage);

	float WeaponCritChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CritChanceDef, EvaluatedParams, WeaponCritChance);

	float WeaponCritDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CritDamageDef, EvaluatedParams, WeaponCritDamage);

	float PlayerDamage = 0.f; //
	UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	PlayerDamage = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UWeaponAttributeSet::GetDamageAttribute());

	float PlayerCritChance = 0.f;
	PlayerCritChance = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UWeaponAttributeSet::GetCritChanceAttribute());
	
	float PlayerCritDamage = 0.f;
	PlayerCritDamage = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UWeaponAttributeSet::GetCritDamageAttribute());

	float PlayerDamageToElite = 0.f;
	PlayerDamageToElite = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UPlayerAttributeSet::GetDamageToElitesAttribute());


	float TotalDamage = 0.f;
	return OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(UCharacterAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -TotalDamage));

}
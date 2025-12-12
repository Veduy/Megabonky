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

	float WeaponCritChance = 0.f; //%퍼센트
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CritChanceDef, EvaluatedParams, WeaponCritChance);

	float WeaponCritDamage = 0.f; // %퍼센트
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CritDamageDef, EvaluatedParams, WeaponCritDamage);

	float PlayerDamage = 0.f; // x배율 최종적으로 계산된 데미지에 곱해줄 값 1.00 ~ 1.xx 
	UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	PlayerDamage = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UPlayerAttributeSet::GetDamageAttribute());

	float PlayerCritChance = 0.f; // %퍼센트
	PlayerCritChance = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UPlayerAttributeSet::GetCritChanceAttribute());
	
	float PlayerCritDamage = 0.f; // x배율
	PlayerCritDamage = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UPlayerAttributeSet::GetCritDamageAttribute());

	float PlayerDamageToElite = 0.f;// x배율
	PlayerDamageToElite = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UPlayerAttributeSet::GetDamageToElitesAttribute());

	// 1.Crit Chance 검사
	// WeaponCritChance + PlayerCritChance

	// 2.Damage 계산 (Crit)
	// 무기에 플레이어 스텟을 적용시킨다.
	// Damage = WeaponDamage *  (1 + (WeaponCritDamage/100)) 
	// Damage = Damage * PlayerCritDamage(배율 2.xx)
	// Damage = Damage * PlayerDamage (최종)

	bool bCrit = false;

	float CritChance = (WeaponCritChance + PlayerCritChance); // 0 ~ X; 
	CritChance >= FMath::RandRange(0, 100) ? bCrit = true : bCrit = false;

	float TotalDamage = 0.f;
	if (bCrit)
	{
		TotalDamage = WeaponDamage * (1 + (WeaponCritDamage) / 100);
		TotalDamage = TotalDamage * PlayerCritDamage;
		TotalDamage = TotalDamage * PlayerDamage;
	}
	else
	{
		TotalDamage = WeaponDamage * PlayerDamage;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("TotalDamage: %f"), TotalDamage);

	return OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(UCharacterAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -TotalDamage));
}
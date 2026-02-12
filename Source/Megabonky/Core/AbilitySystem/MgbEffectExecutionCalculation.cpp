// Copyright is owned by Veduy.


#include "MgbEffectExecutionCalculation.h"

#include "AttributeSet/EnemyAttributeSet.h"
#include "AttributeSet/PlayerAttributeSet.h"
#include "AttributeSet/WeaponAttributeSet.h"

#include "../AbilitySystem/MgbAbilitySystemComponent.h"
#include "../Characters/MgbPlayerCharacter.h"
#include "../Characters/MgbEnemyCharacter.h"
#include "../MgbWeapon.h"
#include "../../Core/MgbPlayerController.h"
#include "../../Util/NetworkLog.h"

UMgbEffectExecutionCalculation::UMgbEffectExecutionCalculation(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Source (Weapon)
	DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, Damage, Source, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, CritChance, Source, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, CritDamage, Source, false);
	
	// Target (EnemyCharacter)
	DEFINE_ATTRIBUTE_CAPTUREDEF(UEnemyAttributeSet, Health, Target, false);

	RelevantAttributesToCapture.Add(DamageDef);
	RelevantAttributesToCapture.Add(CritChanceDef);
	RelevantAttributesToCapture.Add(CritDamageDef);

	RelevantAttributesToCapture.Add(HealthDef);
}

void UMgbEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{	
	// SourceObj(Weapon)->Owner(Player)�� Attribute�� �ʿ�.
	UObject* SourceObj = ExecutionParams.GetOwningSpec().GetEffectContext().GetSourceObject();
	AMgbWeapon* Weapon = Cast<AMgbWeapon>(SourceObj);
	AMgbPlayerCharacter* PlayerCharacter = Cast<AMgbPlayerCharacter>(Weapon->GetOwner());

	FAggregatorEvaluateParameters EvaluatedParams;
	EvaluatedParams.SourceTags = ExecutionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
	EvaluatedParams.TargetTags = ExecutionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

	float WeaponDamage = 0.f; 
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageDef, EvaluatedParams, WeaponDamage);

	float WeaponCritChance = 0.f; // %�ۼ�Ʈ
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CritChanceDef, EvaluatedParams, WeaponCritChance);

	float WeaponCritDamage = 0.f; // %�ۼ�Ʈ
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CritDamageDef, EvaluatedParams, WeaponCritDamage);

	float PlayerDamage = 0.f; // x���� ���������� ���� �������� ������ �� 1.00 ~ 1.xx 
	UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	PlayerDamage = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UPlayerAttributeSet::GetDamageAttribute()) / 100;

	float PlayerCritChance = 0.f; // %�ۼ�Ʈ
	PlayerCritChance = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UPlayerAttributeSet::GetCritChanceAttribute()) / 100;
	
	float PlayerCritDamage = 0.f; // x����
	PlayerCritDamage = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UPlayerAttributeSet::GetCritDamageAttribute()) / 100;

	float PlayerDamageToElite = 0.f;// x����
	PlayerDamageToElite = PlayerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UPlayerAttributeSet::GetDamageToElitesAttribute()) / 100;

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

	// 데미지를 입힌 무기 정보를 적에게 전달 (킬 어트리뷰션용)
	if (Weapon)
	{
		if (ExecutionParams.GetTargetAbilitySystemComponent() == nullptr)
			return;

		AActor* TargetActor = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
		if (TargetActor == nullptr)
			return;

		if (AMgbEnemyCharacter* Enemy = Cast<AMgbEnemyCharacter>(TargetActor))
		{
			Enemy->SetLastDamageWeapon(Weapon);
		}
	}

	if (PlayerCharacter == nullptr)
		return;

	AMgbPlayerController* PC = Cast<AMgbPlayerController>(PlayerCharacter->GetOwner());
	if (PC)
	{
		if (!ExecutionParams.GetTargetAbilitySystemComponent())
			return;

		auto AvatarActor = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
		if (!AvatarActor)
		{
			return;
		}

		FVector Location = AvatarActor->GetActorLocation();
		PC->ClientSpawnDamageTextActor(Location, TotalDamage, bCrit);
	}

	return OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(UEnemyAttributeSet::GetDamageTakenAttribute(), EGameplayModOp::Override, TotalDamage));
}
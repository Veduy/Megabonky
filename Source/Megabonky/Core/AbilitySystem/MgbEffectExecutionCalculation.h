// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayAbilities\Public\GameplayEffectAttributeCaptureDefinition.h"
#include "MgbEffectExecutionCalculation.generated.h"

#define DECLARE_ATTRIBUTE_CAPTUREDEF(P) \
	FProperty* P##Property; \
	FGameplayEffectAttributeCaptureDefinition P##Def; \

#define DEFINE_ATTRIBUTE_CAPTUREDEF(S, P, T, B) \
{ \
	P##Property = FindFieldChecked<FProperty>(S::StaticClass(), GET_MEMBER_NAME_CHECKED(S, P)); \
	P##Def = FGameplayEffectAttributeCaptureDefinition(P##Property, EGameplayEffectAttributeCaptureSource::T, B); \
}

// Source(Weapon)
// ->Damage		//����
// ->CritChance
// ->CritDamage

// PlayerCharacter (Weapon->Owner)
// ->Damage		//����
// ->CritChance	//�ۼ�Ʈ
// ->CritDamage	//
// ->DamageToElite

// Target(Enemy)
// ->Health

UCLASS()
class MEGABONKY_API UMgbEffectExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UMgbEffectExecutionCalculation(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

public:
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

};

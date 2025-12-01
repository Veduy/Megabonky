// Copyright is owned by Veduy.


#include "CharacterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net\UnrealNetwork.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
	InitMaxHealth(100.f);
	InitHealth(100.f);
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, OverHeal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Evasion, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, LifeSteal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Thorns, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, DamageToElites, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, ExtraJumps, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, JumpHeight, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Luck, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Difficulty, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Pickup, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, XPGain, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, GoldGain, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, SilverGain, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, EliteSpawnIncrease, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, PowerupMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, PowerupDropChance, COND_None, REPNOTIFY_Always);
}

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//if (Attribute == GetHealthAttribute())
	//{
	//	NewValue = FMath::Clamp(Health.GetBaseValue(), 0, MaxHealth.GetBaseValue());
	//}
}

void UCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetMaxHealth(FMath::Max(0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Max(0.f, GetHealth()));
	}
}

void UCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
}

void UCharacterAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, HealthRegen, OldHealthRegen);
}

void UCharacterAttributeSet::OnRep_OverHeal(const FGameplayAttributeData& OldOverHeal)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, OverHeal, OldOverHeal);
}

void UCharacterAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Shield, OldShield);
}

void UCharacterAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Armor, OldArmor);
}

void UCharacterAttributeSet::OnRep_Evasion(const FGameplayAttributeData& OldEvasion)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Evasion, OldEvasion);
}

void UCharacterAttributeSet::OnRep_LifeSteal(const FGameplayAttributeData& OldLifeSteal)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, LifeSteal, OldLifeSteal);
}

void UCharacterAttributeSet::OnRep_Thorns(const FGameplayAttributeData& OldThorns)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Thorns, OldThorns);
}

void UCharacterAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UCharacterAttributeSet::OnRep_DamageToElites(const FGameplayAttributeData& OldDamageToElites)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, DamageToElites, OldDamageToElites);
}

void UCharacterAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MovementSpeed, OldMovementSpeed);
}

void UCharacterAttributeSet::OnRep_ExtraJumps(const FGameplayAttributeData& OldExtraJumps)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, ExtraJumps, OldExtraJumps);
}

void UCharacterAttributeSet::OnRep_JumpHeight(const FGameplayAttributeData& OldJumpHeight)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, JumpHeight, OldJumpHeight);
}

void UCharacterAttributeSet::OnRep_Luck(const FGameplayAttributeData& OldLuck)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Luck, OldLuck);
}

void UCharacterAttributeSet::OnRep_Difficulty(const FGameplayAttributeData& OldDifficulty)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Difficulty, OldDifficulty);
}

void UCharacterAttributeSet::OnRep_Pickup(const FGameplayAttributeData& OldPickup)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Pickup, OldPickup);
}

void UCharacterAttributeSet::OnRep_XPGain(const FGameplayAttributeData& OldXPGain)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, XPGain, OldXPGain);
}

void UCharacterAttributeSet::OnRep_GoldGain(const FGameplayAttributeData& OldGoldGain)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, GoldGain, OldGoldGain);
}

void UCharacterAttributeSet::OnRep_SilverGain(const FGameplayAttributeData& OldSilverGain)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, SilverGain, OldSilverGain);
}

void UCharacterAttributeSet::OnRep_EliteSpawnIncrease(const FGameplayAttributeData& OldEliteSpawnIncrease)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, EliteSpawnIncrease, OldEliteSpawnIncrease);
}

void UCharacterAttributeSet::OnRep_PowerupMultiplier(const FGameplayAttributeData& OldPowerupMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, PowerupMultiplier, OldPowerupMultiplier);
}

void UCharacterAttributeSet::OnRep_PowerupDropChance(const FGameplayAttributeData& OldPowerupDropChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, PowerupDropChance, OldPowerupDropChance);
}

// Copyright is owned by Veduy.


#include "PlayerAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net\UnrealNetwork.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
	InitAttackSpeed(1.f);
}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, OverHeal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MaxShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Evasion, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, LifeSteal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Thorns, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, DamageToElites, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, ExtraJumps, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, JumpHeight, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Luck, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Difficulty, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Pickup, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, XPGain, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, GoldGain, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, SilverGain, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, EliteSpawnIncrease, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, PowerupMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, PowerupDropChance, COND_None, REPNOTIFY_Always);
}

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxShield());
	}
}

void UPlayerAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	//Data.Target.
	
	//Data.EvaluatedData.Attribute.
	
	//FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
}

void UPlayerAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, HealthRegen, OldHealthRegen);
}

void UPlayerAttributeSet::OnRep_OverHeal(const FGameplayAttributeData& OldOverHeal)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, OverHeal, OldOverHeal);
}

void UPlayerAttributeSet::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MaxShield, OldMaxShield);
}

void UPlayerAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield)  
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Shield, OldShield);
}

void UPlayerAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Armor, OldArmor);
}

void UPlayerAttributeSet::OnRep_Evasion(const FGameplayAttributeData& OldEvasion)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Evasion, OldEvasion);
}

void UPlayerAttributeSet::OnRep_LifeSteal(const FGameplayAttributeData& OldLifeSteal)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, LifeSteal, OldLifeSteal);
}

void UPlayerAttributeSet::OnRep_Thorns(const FGameplayAttributeData& OldThorns)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Thorns, OldThorns);
}

void UPlayerAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UPlayerAttributeSet::OnRep_DamageToElites(const FGameplayAttributeData& OldDamageToElites)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, DamageToElites, OldDamageToElites);
}

void UPlayerAttributeSet::OnRep_ExtraJumps(const FGameplayAttributeData& OldExtraJumps)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, ExtraJumps, OldExtraJumps);
}

void UPlayerAttributeSet::OnRep_JumpHeight(const FGameplayAttributeData& OldJumpHeight)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, JumpHeight, OldJumpHeight);
}

void UPlayerAttributeSet::OnRep_Luck(const FGameplayAttributeData& OldLuck)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Luck, OldLuck);
}

void UPlayerAttributeSet::OnRep_Difficulty(const FGameplayAttributeData& OldDifficulty)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Difficulty, OldDifficulty);
}

void UPlayerAttributeSet::OnRep_Pickup(const FGameplayAttributeData& OldPickup)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Pickup, OldPickup);
}

void UPlayerAttributeSet::OnRep_XPGain(const FGameplayAttributeData& OldXPGain)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, XPGain, OldXPGain);
}

void UPlayerAttributeSet::OnRep_GoldGain(const FGameplayAttributeData& OldGoldGain)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, GoldGain, OldGoldGain);
}

void UPlayerAttributeSet::OnRep_SilverGain(const FGameplayAttributeData& OldSilverGain)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, SilverGain, OldSilverGain);
}

void UPlayerAttributeSet::OnRep_EliteSpawnIncrease(const FGameplayAttributeData& OldEliteSpawnIncrease)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, EliteSpawnIncrease, OldEliteSpawnIncrease);
}

void UPlayerAttributeSet::OnRep_PowerupMultiplier(const FGameplayAttributeData& OldPowerupMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, PowerupMultiplier, OldPowerupMultiplier);
}

void UPlayerAttributeSet::OnRep_PowerupDropChance(const FGameplayAttributeData& OldPowerupDropChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, PowerupDropChance, OldPowerupDropChance);
}

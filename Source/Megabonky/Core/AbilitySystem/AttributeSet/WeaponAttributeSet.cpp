// Copyright is owned by Veduy.


#include "WeaponAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net\UnrealNetwork.h"

UWeaponAttributeSet::UWeaponAttributeSet()
{

}

void UWeaponAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, CritDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, ProjectileCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, ProjectileSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, ProjectileBounces, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, Size, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, Knockback, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, Duration, COND_None, REPNOTIFY_Always);
}

void UWeaponAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{

}

void UWeaponAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, Damage, OldDamage);
}

void UWeaponAttributeSet::OnRep_CritChance(const FGameplayAttributeData& OldCritChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, CritChance, OldCritChance);
}

void UWeaponAttributeSet::OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, CritDamage, OldCritDamage);
}

void UWeaponAttributeSet::OnRep_ProjectileCount(const FGameplayAttributeData& OldProjectileCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, ProjectileCount, OldProjectileCount);
}

void UWeaponAttributeSet::OnRep_ProjectileSpeed(const FGameplayAttributeData& OldProjectileSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, ProjectileSpeed, OldProjectileSpeed);
}

void UWeaponAttributeSet::OnRep_ProjectileBounces(const FGameplayAttributeData& OldProjectileBounces)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, ProjectileBounces, OldProjectileBounces);
}

void UWeaponAttributeSet::OnRep_Size(const FGameplayAttributeData& OldSize)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, Size, OldSize);
}

void UWeaponAttributeSet::OnRep_Knockback(const FGameplayAttributeData& OldKnockback)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, Knockback, OldKnockback);
}

void UWeaponAttributeSet::OnRep_Duration(const FGameplayAttributeData& OldDuration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, Duration, OldDuration);
}

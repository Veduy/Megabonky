// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class MEGABONKY_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPlayerAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:	
	UFUNCTION()
	virtual void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen);

	UFUNCTION()
	virtual void OnRep_OverHeal(const FGameplayAttributeData& OldOverHeal);

	UFUNCTION()
	virtual void OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield);

	UFUNCTION()
	virtual void OnRep_Shield(const FGameplayAttributeData& OldShield);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);
	
	UFUNCTION()
	virtual void OnRep_Evasion(const FGameplayAttributeData& OldEvasion);

	UFUNCTION()
	virtual void OnRep_LifeSteal(const FGameplayAttributeData& OldLifeSteal);

	UFUNCTION()
	virtual void OnRep_Thorns(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed);

	UFUNCTION()
	virtual void OnRep_DamageToElites(const FGameplayAttributeData& OldDamageToElites);

	UFUNCTION()
	virtual void OnRep_ExtraJumps(const FGameplayAttributeData& OldExtraJumps);

	UFUNCTION()
	virtual void OnRep_JumpHeight(const FGameplayAttributeData& OldJumpHeight);

	UFUNCTION()
	virtual void OnRep_Luck(const FGameplayAttributeData& OldLuck); 

	UFUNCTION()
	virtual void OnRep_Difficulty(const FGameplayAttributeData& OldDifficulty);

	UFUNCTION()
	virtual void OnRep_Pickup(const FGameplayAttributeData& OldPickup);

	UFUNCTION()
	virtual void OnRep_XPGain(const FGameplayAttributeData& OldXPGain);

	UFUNCTION()
	virtual void OnRep_GoldGain(const FGameplayAttributeData& OldGoldGain);

	UFUNCTION()
	virtual void OnRep_SilverGain(const FGameplayAttributeData& OldSilverGain);

	UFUNCTION()
	virtual void OnRep_EliteSpawnIncrease(const FGameplayAttributeData& OldEliteSpawnIncrease);

	UFUNCTION()
	virtual void OnRep_PowerupMultiplier(const FGameplayAttributeData& OldPowerupMultiplier);

	UFUNCTION()
	virtual void OnRep_PowerupDropChance(const FGameplayAttributeData& OldPowerupDropChance);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, HealthRegen);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_OverHeal)
	FGameplayAttributeData OverHeal;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, OverHeal);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxShield)
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MaxShield);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Shield);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Armor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Evasion)
	FGameplayAttributeData Evasion;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Evasion);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_LifeSteal)
	FGameplayAttributeData LifeSteal;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, LifeSteal);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Thorns)
	FGameplayAttributeData Thorns;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Thorns);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, AttackSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_DamageToElites)
	FGameplayAttributeData DamageToElites;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, DamageToElites);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ExtraJumps)
	FGameplayAttributeData ExtraJumps;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, ExtraJumps);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_JumpHeight)
	FGameplayAttributeData JumpHeight;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, JumpHeight);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Luck)
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Luck);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Difficulty)
	FGameplayAttributeData Difficulty;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Difficulty);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Pickup)
	FGameplayAttributeData Pickup;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Pickup);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_XPGain)
	FGameplayAttributeData XPGain;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, XPGain);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_GoldGain)
	FGameplayAttributeData GoldGain;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, GoldGain);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SilverGain)
	FGameplayAttributeData SilverGain;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, SilverGain);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_EliteSpawnIncrease)
	FGameplayAttributeData EliteSpawnIncrease;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, EliteSpawnIncrease);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_PowerupMultiplier)
	FGameplayAttributeData PowerupMultiplier;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, PowerupMultiplier);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_PowerupDropChance)
	FGameplayAttributeData PowerupDropChance;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, PowerupDropChance);
};
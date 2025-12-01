// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class MEGABONKY_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UCharacterAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;


public:
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UFUNCTION()
	virtual void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen);

	UFUNCTION()
	virtual void OnRep_OverHeal(const FGameplayAttributeData& OldOverHeal);

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
	virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HealthRegen);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_OverHeal)
	FGameplayAttributeData OverHeal;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, OverHeal);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Shield);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Armor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Evasion)
	FGameplayAttributeData Evasion;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Evasion);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_LifeSteal)
	FGameplayAttributeData LifeSteal;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, LifeSteal);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Thorns)
	FGameplayAttributeData Thorns;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Thorns);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, AttackSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_DamageToElites)
	FGameplayAttributeData DamageToElites;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, DamageToElites);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MovementSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ExtraJumps)
	FGameplayAttributeData ExtraJumps;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ExtraJumps);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_JumpHeight)
	FGameplayAttributeData JumpHeight;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, JumpHeight);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Luck)
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Luck);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Difficulty)
	FGameplayAttributeData Difficulty;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Difficulty);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Pickup)
	FGameplayAttributeData Pickup;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Pickup);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_XPGain)
	FGameplayAttributeData XPGain;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, XPGain);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_GoldGain)
	FGameplayAttributeData GoldGain;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, GoldGain);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SilverGain)
	FGameplayAttributeData SilverGain;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, SilverGain);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_EliteSpawnIncrease)
	FGameplayAttributeData EliteSpawnIncrease;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, EliteSpawnIncrease);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_PowerupMultiplier)
	FGameplayAttributeData PowerupMultiplier;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, PowerupMultiplier);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_PowerupDropChance)
	FGameplayAttributeData PowerupDropChance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, PowerupDropChance);



};




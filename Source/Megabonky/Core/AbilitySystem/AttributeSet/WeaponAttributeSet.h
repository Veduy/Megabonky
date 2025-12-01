// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WeaponAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class MEGABONKY_API UWeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWeaponAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	UFUNCTION()
	virtual void OnRep_Damage(const FGameplayAttributeData& OldDamage);

	UFUNCTION()
	virtual void OnRep_CritChance(const FGameplayAttributeData& OldCritChance);

	UFUNCTION()
	virtual void OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage);

	UFUNCTION()
	virtual void OnRep_ProjectileCount(const FGameplayAttributeData& OldProjectileCount);

	UFUNCTION()
	virtual void OnRep_ProjectileSpeed(const FGameplayAttributeData& OldProjectileSpeed);

	UFUNCTION()
	virtual void OnRep_ProjectileBounces(const FGameplayAttributeData& OldProjectileBounces);

	UFUNCTION()
	virtual void OnRep_Size(const FGameplayAttributeData& OldSize);

	UFUNCTION()
	virtual void OnRep_Knockback(const FGameplayAttributeData& OldKnockback);

	UFUNCTION()
	virtual void OnRep_Duration(const FGameplayAttributeData& OldDuration);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, Damage);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_CritChance)
	FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, CritChance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_CritDamage)
	FGameplayAttributeData CritDamage;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, CritDamage);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ProjectileCount)
	FGameplayAttributeData ProjectileCount;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, ProjectileCount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ProjectileSpeed)
	FGameplayAttributeData ProjectileSpeed;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, ProjectileSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ProjectileBounces)
	FGameplayAttributeData ProjectileBounces;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, ProjectileBounces);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Size)
	FGameplayAttributeData Size;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, Size);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Knockback)
	FGameplayAttributeData Knockback;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, Knockback);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Duration)
	FGameplayAttributeData Duration;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, Duration);
	
};

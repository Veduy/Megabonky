// Copyright is owned by Veduy.


#include "EnemyAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net\UnrealNetwork.h"

#include "../../Characters/MgbEnemyCharacter.h"
#include "../../MgbGameplayTags.h"
#include "../../../Util/NetworkLog.h"

UEnemyAttributeSet::UEnemyAttributeSet()
{
	InitMaxHealth(10.f);
	InitHealth(10.f);
	InitDamageTaken(0.f);
}

void UEnemyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEnemyAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEnemyAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEnemyAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
}

void UEnemyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}
}

void UEnemyAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		if(OldValue > NewValue)
		{
			AMgbEnemyCharacter* Character = Cast<AMgbEnemyCharacter>(GetOwningActor());
			if (Character)
			{
				auto ASC = GetOwningAbilitySystemComponent();
				if (!ASC)
					return;

				ASC->ExecuteGameplayCue(TAG_GameplayCue_EC_Damaged);

				if (NewValue <= 0.f)
				{
					GetOwningActor()->SetLifeSpan(0.2f);
				}
			}
	
		}
	
	}
}

void UEnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		float FinalDamage = GetDamageTaken();
		SetDamageTaken(0.f);

		float OldHealth = GetHealth();
		float NewHealth = FMath::Clamp(OldHealth - FinalDamage, 0.f, GetMaxHealth());

		SetHealth(NewHealth);
	} 
}

void UEnemyAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnemyAttributeSet, MaxHealth, OldMaxHealth);
}

void UEnemyAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnemyAttributeSet, Health, OldHealth);

	if (GetHealth() <= OldHealth.GetCurrentValue())
	{

	}
}

void UEnemyAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnemyAttributeSet, MovementSpeed, OldMovementSpeed);
}

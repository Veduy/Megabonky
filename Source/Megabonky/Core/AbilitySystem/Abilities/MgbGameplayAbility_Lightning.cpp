// Copyright is owned by Veduy.


#include "MgbGameplayAbility_Lightning.h"

void UMgbGameplayAbility_Lightning::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	/*
		1.일정 범위 내 적 찾기.
		2.적에게 데미지 effect 적용.

	*/

}

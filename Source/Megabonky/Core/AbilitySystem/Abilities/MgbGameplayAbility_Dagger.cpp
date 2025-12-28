// Copyright is owned by Veduy.


#include "MgbGameplayAbility_Dagger.h"

void UMgbGameplayAbility_Dagger::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	RapidFire();
}

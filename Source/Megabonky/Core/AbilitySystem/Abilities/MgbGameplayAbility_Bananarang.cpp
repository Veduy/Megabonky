// Copyright is owned by Veduy.


#include "MgbGameplayAbility_Bananarang.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "../../../Util/NetworkLog.h"


void UMgbGameplayAbility_Bananarang::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	NET_LOG("");
	RapidFire();
}
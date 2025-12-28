// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "MgbGameplayAbility_Projectile.h"
#include "MgbGameplayAbility_Dagger.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKY_API UMgbGameplayAbility_Dagger : public UMgbGameplayAbility_Projectile
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};

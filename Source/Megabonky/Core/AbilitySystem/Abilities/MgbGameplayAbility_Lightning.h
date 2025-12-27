// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "MgbGameplayAbility.h"
#include "MgbGameplayAbility_Lightning.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKY_API UMgbGameplayAbility_Lightning : public UMgbGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
public:
	UFUNCTION()
	void Lightning();

public:
	UPROPERTY()
	uint8 LightingCount = 0;
};

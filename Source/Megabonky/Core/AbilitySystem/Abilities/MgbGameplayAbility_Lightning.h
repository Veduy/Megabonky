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

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
    UFUNCTION()
    void Lightning();

public:
    UPROPERTY()
    uint8 LightingCount = 0;

    // 모든 활성 타이머 정리용 (비정상 종료 시 사용)
    FTimerHandle LightingTimerHandle;
};
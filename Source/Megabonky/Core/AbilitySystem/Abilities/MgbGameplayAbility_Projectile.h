// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "MgbGameplayAbility.h"
#include "MgbGameplayAbility_Projectile.generated.h"

class AMgbProjectileActor;
/**
 * 
 */
UCLASS()
class MEGABONKY_API UMgbGameplayAbility_Projectile : public UMgbGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AMgbProjectileActor> ProjectileClass;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Projectile");
	uint8 SpawnProjectileCount = 1;
};

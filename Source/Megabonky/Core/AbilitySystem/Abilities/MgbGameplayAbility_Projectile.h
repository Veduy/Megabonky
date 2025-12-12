// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "MgbGameplayAbility.h"
#include "MgbGameplayAbility_Projectile.generated.h"
  
class AMgbProjectileActor;
/**
 *
 */

UCLASS(Blueprintable, BlueprintType)
class MEGABONKY_API UMgbGameplayAbility_Projectile : public UMgbGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	void SpawnProjectile(AActor* Owner, const FVector& InSpawnOrigin, const FVector& InSpawnDir);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AMgbProjectileActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile");
	uint8 SpawnProjectileCount = 1;

	uint8 TempSpawnCount = 0;

	FTimerHandle SpawnTimerHandle;
};

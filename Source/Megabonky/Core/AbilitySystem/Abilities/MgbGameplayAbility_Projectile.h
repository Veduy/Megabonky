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
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void RapidFire();

	UFUNCTION(BlueprintCallable)
	virtual void SpawnProjectile(AActor* Owner, const FVector& InSpawnOrigin, const FRotator& InSpawnDir);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<AMgbProjectileActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data");
	uint8 SpawnProjectileCount = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data");
	uint8 ProjectileBounceCount = 0;

	uint8 CurrentSpawnCount = 0;

	FTimerHandle SpawnTimerHandle;
};

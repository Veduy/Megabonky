// Copyright is owned by Veduy.


#include "MgbGameplayAbility_Projectile.h"

#include "Kismet/GameplayStatics.h"
#include "../../../Actors/MgbProjectileActor.h"

void UMgbGameplayAbility_Projectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FTransform SpawnTransform = FTransform(FVector(0.f, 0.f, 0.f));
	AMgbProjectileActor* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AMgbProjectileActor>(
		ProjectileClass,
		SpawnTransform, 
		ActorInfo->OwnerActor.Get(),
		Cast<APawn>(ActorInfo->OwnerActor.Get()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		ESpawnActorScaleMethod::MultiplyWithRoot);

	if (SpawnedProjectile)
	{
		// 초기 설정할게 있다면 여기에 추가.
		UGameplayStatics::FinishSpawningActor(SpawnedProjectile, SpawnTransform);
	}
}

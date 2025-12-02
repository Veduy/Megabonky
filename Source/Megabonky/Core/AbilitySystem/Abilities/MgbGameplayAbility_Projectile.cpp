// Copyright is owned by Veduy.


#include "MgbGameplayAbility_Projectile.h"

#include "Kismet/GameplayStatics.h"
#include "../../../Actors/MgbProjectileActor.h"

void UMgbGameplayAbility_Projectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FTransform SpawnTransform = FTransform(FVector(0.f, 0.f, 0.f));
	Projectile = GetWorld()->SpawnActorDeferred<AMgbProjectileActor>(
		ProjectileClass,
		SpawnTransform);

	if (Projectile)
	{
		// 스폰 완료전 데이터 세팅.

		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	}
}

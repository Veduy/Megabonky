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
		SpawnTransform, 
		ActorInfo->OwnerActor.Get(),
		Cast<APawn>(ActorInfo->OwnerActor.Get()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		ESpawnActorScaleMethod::MultiplyWithRoot);

	if (Projectile)
	{
		// 스폰 완료전 데이터 설정할게 생기면 추가.
		// Projectile Actor에 Effect를 변수로 가

		UE_LOG(LogTemp, Warning, TEXT("\"%s\"'s Owner : %s"), *GetName(), *ActorInfo->OwnerActor->GetName());

		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	}
}

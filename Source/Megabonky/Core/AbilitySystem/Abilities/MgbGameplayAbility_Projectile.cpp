// Copyright is owned by Veduy.


#include "MgbGameplayAbility_Projectile.h"

#include "Kismet/GameplayStatics.h"
#include "../../../Actors/MgbProjectileActor.h"

#include "../../../Util/NetworkLog.h"
#include "../../Characters/MgbPlayerCharacter.h"

void UMgbGameplayAbility_Projectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Avatar = Weapon Actor
	AActor* Avatar = ActorInfo->AvatarActor.Get();

	// Player
	AActor* Player = Avatar->GetOwner();

	//Avatar->GetOwner();
	FVector SpawnLocation;
	FVector SpawnDirection;
	
	if (Player)
	{
		SpawnLocation = Player->GetActorLocation();
		SpawnDirection = Player->GetActorForwardVector();
	}

	FTransform SpawnTransform = FTransform(SpawnDirection.Rotation(), SpawnLocation, FVector(1.f,1.f,1.f));
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

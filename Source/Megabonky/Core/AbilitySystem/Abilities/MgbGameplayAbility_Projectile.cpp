// Copyright is owned by Veduy.


#include "MgbGameplayAbility_Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "../../../Actors/MgbProjectileActor.h"
#include "../../../Util/NetworkLog.h"
#include "../../Characters/MgbPlayerCharacter.h"
#include "../../Characters/MgbEnemyCharacter.h"
#include "../AttributeSet/WeaponAttributeSet.h"
#include "../AttributeSet/PlayerAttributeSet.h"

void UMgbGameplayAbility_Projectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


}

void UMgbGameplayAbility_Projectile::SpawnProjectile(AActor* Owner, const FVector& InSpawnOrigin, const FRotator& InSpawnDir)
{
	FTransform SpawnTransform = FTransform(InSpawnDir, InSpawnOrigin, FVector(1.f, 1.f, 1.f));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMgbProjectileActor* Projectile = GetWorld()->SpawnActor<AMgbProjectileActor>(ProjectileClass, SpawnTransform, SpawnParams);
	Projectile->BounceCount = ProjectileBounceCount;

	CurrentSpawnCount++;
}

void UMgbGameplayAbility_Projectile::RapidFire()
{
	AActor* WeaponActor = GetCurrentActorInfo()->AvatarActor.Get(); // Avatar = Weapon Actor
	UAbilitySystemComponent* WeaponASC = GetAbilitySystemComponentFromActorInfo();

	AActor* PlayerActor = WeaponActor->GetOwner(); // PlayerActor
	UAbilitySystemComponent* PlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerActor);

	// Weapon ProjectileCount + Player ProjectileCount
	uint8 WeaponProjectileCount = WeaponASC->GetNumericAttribute(UWeaponAttributeSet::GetProjectileCountAttribute());
	uint8 PlayerProjectileCount = PlayerASC->GetNumericAttribute(UPlayerAttributeSet::GetProjectileCountAttribute());
	SpawnProjectileCount = WeaponProjectileCount + PlayerProjectileCount;

	// Weapon ProjectileBounce + Player ProjectileBounce
	uint8 WeaponProjectileBounce = WeaponASC->GetNumericAttribute(UWeaponAttributeSet::GetProjectileBouncesAttribute());
	uint8 PlayerProjectileBounce = PlayerASC->GetNumericAttribute(UPlayerAttributeSet::GetProjectileBouncesAttribute());
	ProjectileBounceCount = WeaponProjectileBounce + PlayerProjectileBounce;

	float SpawnInterval = float(1) / float(SpawnProjectileCount);

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle,
		[this, PlayerActor, WeaponActor]()
		{
			AActor* TargetActor = nullptr;
			// 스폰 방향을 PrimaryTarget 으로 수정.
			AMgbPlayerCharacter* PlayerCharacter = Cast<AMgbPlayerCharacter>(PlayerActor);
			if (PlayerCharacter)
			{
				PlayerCharacter->FindPrimaryTargetByCondition(TargetActor);
			}

			if (TargetActor && Cast<AMgbEnemyCharacter>(TargetActor)->bSpawnFinished)
			{
				FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PlayerActor->GetActorLocation(), TargetActor->GetActorLocation());

				SpawnProjectile(WeaponActor, PlayerActor->GetActorLocation(), TargetRotation);
			}
			else
			{
				CurrentSpawnCount++;
			}

			if (CurrentSpawnCount >= SpawnProjectileCount)
			{
				CurrentSpawnCount = 0;
				GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);

				EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
			}
		},
		SpawnInterval,
		true,
		0.f);
}
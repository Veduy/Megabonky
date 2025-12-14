// Copyright is owned by Veduy.


#include "MgbGameplayAbility_Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "../../../Actors/MgbProjectileActor.h"
#include "../../../Util/NetworkLog.h"
#include "../../Characters/MgbPlayerCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "../AttributeSet/WeaponAttributeSet.h"
#include "../AttributeSet/PlayerAttributeSet.h"

void UMgbGameplayAbility_Projectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//RapidFire();

	/* 수정중 
	// Avatar = Weapon Actor
	AActor* WeaponActor = ActorInfo->AvatarActor.Get();

	// PlayerActor
	AActor* PlayerActor = WeaponActor->GetOwner();

	// Weapon ProjectileCount + Player ProjectileCount
	UAbilitySystemComponent* WeaponASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(WeaponActor);
	uint8 WeaponProjectileCount = WeaponASC->GetNumericAttribute(UWeaponAttributeSet::GetProjectileCountAttribute());

	UAbilitySystemComponent* PlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerActor);
	uint8 PlayerProjectileCount = PlayerASC->GetNumericAttribute(UPlayerAttributeSet::GetProjectileCountAttribute());

	SpawnProjectileCount = WeaponProjectileCount + PlayerProjectileCount;

	// Player AttackSpeed / SpawnProjectileCount
	// ex) 1.f / 5 = 0.2초 간격으로 스폰. 
	float PlayerAttackSpeed = PlayerASC->GetNumericAttribute(UPlayerAttributeSet::GetAttackSpeedAttribute()); // 100% ~ x%;
	PlayerAttackSpeed = PlayerAttackSpeed / 100.f;

	float SpawnInterval = PlayerAttackSpeed / SpawnProjectileCount;

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle,
		[this, PlayerActor, WeaponActor, Handle, ActorInfo, ActivationInfo]()
		{
			SpawnProjectile(WeaponActor, PlayerActor->GetActorLocation(), PlayerActor->GetActorForwardVector());

			if (TempSpawnCount >= SpawnProjectileCount)
			{
				TempSpawnCount = 0;
				GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
				EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
			}
		},
		SpawnInterval,
		true);
	*/
}

void UMgbGameplayAbility_Projectile::SpawnProjectile(AActor* Owner, const FVector& InSpawnOrigin, const FVector& InSpawnDir)
{
	FTransform SpawnTransform = FTransform(InSpawnDir.Rotation(), InSpawnOrigin, FVector(1.f, 1.f, 1.f));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AMgbProjectileActor>(ProjectileClass, SpawnTransform, SpawnParams);
	TempSpawnCount++;
}

void UMgbGameplayAbility_Projectile::RapidFire()
{
	UAbilitySystemComponent* WeaponASC = GetAbilitySystemComponentFromActorInfo();

	CurrentActivationInfo;

	// Avatar = Weapon Actor
	GetActorInfo();
	AActor* WeaponActor = GetCurrentActorInfo()->AvatarActor.Get();

	// PlayerActor
	AActor* PlayerActor = WeaponActor->GetOwner();

	// Weapon ProjectileCount + Player ProjectileCount
	//UAbilitySystemComponent* WeaponASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(WeaponActor);
	uint8 WeaponProjectileCount = WeaponASC->GetNumericAttribute(UWeaponAttributeSet::GetProjectileCountAttribute());

	UAbilitySystemComponent* PlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerActor);
	uint8 PlayerProjectileCount = PlayerASC->GetNumericAttribute(UPlayerAttributeSet::GetProjectileCountAttribute());

	SpawnProjectileCount = WeaponProjectileCount + PlayerProjectileCount;

	// Player AttackSpeed / SpawnProjectileCount
	// ex) 1.f / 5 = 0.2초 간격으로 스폰. 
	float PlayerAttackSpeed = PlayerASC->GetNumericAttribute(UPlayerAttributeSet::GetAttackSpeedAttribute()); // 100% ~ x%;
	PlayerAttackSpeed = PlayerAttackSpeed / 100.f;

	float SpawnInterval = PlayerAttackSpeed / SpawnProjectileCount;

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle,
		[this, PlayerActor, WeaponActor /*Handle, ActorInfo, ActivationInfo*/]()
		{
			SpawnProjectile(WeaponActor, PlayerActor->GetActorLocation(), PlayerActor->GetActorForwardVector());

			if (TempSpawnCount >= SpawnProjectileCount)
			{
				TempSpawnCount = 0;
				GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);

				EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
			}
		},
		SpawnInterval,
		true);
}
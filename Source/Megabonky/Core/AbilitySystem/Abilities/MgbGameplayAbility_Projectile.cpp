// Copyright is owned by Veduy.


#include "MgbGameplayAbility_Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	FRotator Rotation = InSpawnDir;
	Rotation.Roll = 0.f;

	// ������ ������ ���� + ĳ���� ������ ����
	UAbilitySystemComponent* WeaponASC = GetAbilitySystemComponentFromActorInfo();
	auto WeaponSize = WeaponASC->GetNumericAttribute(UWeaponAttributeSet::GetSizeAttribute()) / 100;
	auto WeaponProjSpeed = WeaponASC->GetNumericAttribute(UWeaponAttributeSet::GetProjectileSpeedAttribute()); // ���� ��

	UAbilitySystemComponent* PlayerASC = GetPlayerASC();
	auto PlayerSize = PlayerASC->GetNumericAttribute(UPlayerAttributeSet::GetSizeAttribute()) / 100;
	auto PlayerProjSpeed = PlayerASC->GetNumericAttribute(UPlayerAttributeSet::GetProjectileSpeedAttribute()) / 100; //�ۼ�Ʈ

	auto Size = WeaponSize * PlayerSize;
	auto ProjSpeed = WeaponProjSpeed * PlayerProjSpeed;

	FTransform SpawnTransform = FTransform(Rotation, InSpawnOrigin, FVector(1, 1, 1));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (ProjectileClass)
	{
		AMgbProjectileActor* Projectile = GetWorld()->SpawnActor<AMgbProjectileActor>(ProjectileClass, SpawnTransform, SpawnParams);
		if (Projectile)
		{
			Projectile->SetActorScale3D(FVector(Size, Size, Size));
			Projectile->BounceCount = ProjectileBounceCount;
			Projectile->ProjectileMovement->InitialSpeed = ProjSpeed;
			Projectile->ProjectileMovement->MaxSpeed = ProjSpeed;
		}
	}

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
			// ���� ������ PrimaryTarget ���� ����.
			AMgbPlayerCharacter* PlayerCharacter = Cast<AMgbPlayerCharacter>(PlayerActor);

			bool bFoundTarget = false;
			if (PlayerCharacter)
			{
				bFoundTarget = PlayerCharacter->FindPrimaryTargetByCondition(TargetActor);
			}

			if (bFoundTarget && TargetActor && Cast<AMgbEnemyCharacter>(TargetActor)->bSpawnFinished)
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

void UMgbGameplayAbility_Projectile::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Ability 종료 시 타이머 안전하게 정리 (비정상 종료 시에도 정리됨)
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
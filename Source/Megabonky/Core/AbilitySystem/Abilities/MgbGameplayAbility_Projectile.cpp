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

	UAbilitySystemComponent* WeaponASC = GetAbilitySystemComponentFromActorInfo();
	auto WeaponSize = WeaponASC->GetNumericAttribute(UWeaponAttributeSet::GetSizeAttribute()) / 100;
	auto WeaponProjSpeed = WeaponASC->GetNumericAttribute(UWeaponAttributeSet::GetProjectileSpeedAttribute()); 

	UAbilitySystemComponent* PlayerASC = GetPlayerASC();
	auto PlayerSize = PlayerASC->GetNumericAttribute(UPlayerAttributeSet::GetSizeAttribute()) / 100;
	auto PlayerProjSpeed = PlayerASC->GetNumericAttribute(UPlayerAttributeSet::GetProjectileSpeedAttribute()) / 100; 

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

	// [Fix] 안전하게 어빌리티와 액터들의 소멸을 감지하기 위한 Weak 캡처 포인터
	TWeakObjectPtr<UMgbGameplayAbility_Projectile> WeakThis(this);
	TWeakObjectPtr<AActor> WeakPlayerActor(PlayerActor);
	TWeakObjectPtr<AActor> WeakWeaponActor(WeaponActor);

	// [Fix] 생포인터(this) 대신 CreateWeakLambda를 사용하여 어빌리티 파괴 시 타이머가 참조 위반을 일으키지 않도록 방어
	FTimerDelegate Delegate = FTimerDelegate::CreateWeakLambda(this, [WeakThis, WeakPlayerActor, WeakWeaponActor]()
	{
		UMgbGameplayAbility_Projectile* StrongThis = WeakThis.Get();
		if (!StrongThis || !StrongThis->IsActive())
		{
			return;
		}

		AActor* PlayerActor = WeakPlayerActor.Get();
		AActor* WeaponActor = WeakWeaponActor.Get();

		// [Fix] 플레이어 또는 무기 액터가 타이머 반복 도중 파괴되었을 경우 능력(Ability) 강제 종료
		if (!IsValid(PlayerActor) || !IsValid(WeaponActor))
		{
			StrongThis->CurrentSpawnCount = StrongThis->SpawnProjectileCount; // Force end ability
		}
		else
		{
			AActor* TargetActor = nullptr;
			AMgbPlayerCharacter* PlayerCharacter = Cast<AMgbPlayerCharacter>(PlayerActor);

			bool bFoundTarget = false;
			if (PlayerCharacter)
			{
				bFoundTarget = PlayerCharacter->FindPrimaryTargetByCondition(TargetActor);
			}

			// [Fix] 캐스팅 후 IsValid를 통해 Enemy 포인터가 유효한지 안전하게 검사 (널 포인터 참조 크래시 방지)
			AMgbEnemyCharacter* EnemyTarget = Cast<AMgbEnemyCharacter>(TargetActor);
			if (bFoundTarget && IsValid(EnemyTarget) && EnemyTarget->bSpawnFinished)
			{
				FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PlayerActor->GetActorLocation(), EnemyTarget->GetActorLocation());
				StrongThis->SpawnProjectile(WeaponActor, PlayerActor->GetActorLocation(), TargetRotation);
			}
			else
			{
				StrongThis->CurrentSpawnCount++;
			}
		}

		if (StrongThis->CurrentSpawnCount >= StrongThis->SpawnProjectileCount)
		{
			StrongThis->CurrentSpawnCount = 0;
			if (UWorld* World = StrongThis->GetWorld())
			{
				World->GetTimerManager().ClearTimer(StrongThis->SpawnTimerHandle);
			}

			StrongThis->EndAbility(StrongThis->GetCurrentAbilitySpecHandle(), StrongThis->GetCurrentActorInfo(), StrongThis->GetCurrentActivationInfo(), false, false);
		}
	});

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, Delegate, SpawnInterval, true, 0.f);
}

void UMgbGameplayAbility_Projectile::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Ability 종료 시 타이머 안전하게 정리
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
// Copyright is owned by Veduy.


#include "Projectile_Bananarang.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "../../Core/MgbWeapon.h"
#include "../../Core/Characters/MgbEnemyCharacter.h"

void AProjectile_Bananarang::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return;
	}

	SetLifeSpan(0.f);

	GetWorldTimerManager().SetTimer(
		OutBoundTimer,
		this,
		&AProjectile_Bananarang::OutBoundFlight,
		OutBoundFlightTime,
		false
	);
}

void AProjectile_Bananarang::BeginOverlap(AActor* OtherActor)
{
	// 서버에서만 충돌 검사.
	if (!HasAuthority())
	{
		return;
	}

	// GetOwner() = WeaponActor;
	AMgbWeapon* Weapon = Cast<AMgbWeapon>(GetOwner());

	if (bReturning && OtherActor == Weapon->GetOwner())
	{
		Destroy();
	}

	AMgbEnemyCharacter* Enemy = Cast<AMgbEnemyCharacter>(OtherActor);
	if (Enemy)
	{	
		if (bRadialDamage == false)
		{
			if (Weapon)
			{
				FGameplayEffectContextHandle EffectContextHandle = Weapon->GetAbilitySystemComponent()->MakeEffectContext();
				EffectContextHandle.AddSourceObject(Weapon);
				EffectContextHandle.AddInstigator(Weapon, Weapon);

				if (Weapon->DamageEffectClass)
				{
					// Spec을 생성한 컴포넌트가 ExecCalc의 Source로 설정.
					FGameplayEffectSpecHandle EffectSpecHandle = Weapon->GetAbilitySystemComponent()->MakeOutgoingSpec(Weapon->DamageEffectClass, 1.f, EffectContextHandle);

					UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
					Weapon->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
				}
			}
		}

		// 처음 날라가는 중이었다면, 스탑.
		if (OutBoundTimer.IsValid())
		{
			if (GetWorldTimerManager().TimerExists(OutBoundTimer))
			{
				GetWorldTimerManager().ClearTimer(OutBoundTimer);
				OutBoundFlight();
			}
		}
	}
}

void AProjectile_Bananarang::OutBoundFlight()
{
	if (!HasAuthority())
	{
		return;
	}

	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->Deactivate();

	FTimerHandle StayTimer;
	GetWorldTimerManager().SetTimer(
		StayTimer,
		this,
		&AProjectile_Bananarang::ReturnToPlayer,
		1.f,
		false
	);
}

void AProjectile_Bananarang::ReturnToPlayer()
{
	if (HasAuthority())
	{
		bReturning = true;

		ProjectileMovement->Activate();
		ProjectileMovement->bIsHomingProjectile = true;

		// GetOwner()->GetOwner() = PlayerCharacter;
		if (IsValid(GetOwner()))
		{
			ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner()->GetOwner());
			if (PlayerCharacter)
			{
				ProjectileMovement->HomingTargetComponent = PlayerCharacter->GetRootComponent();
				ProjectileMovement->HomingAccelerationMagnitude = 15000.f;
			}
		}
	}
}
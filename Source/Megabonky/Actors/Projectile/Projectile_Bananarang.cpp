// Copyright is owned by Veduy.


#include "Projectile_Bananarang.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "../../Core/MgbWeapon.h"
#include "../../Core/Characters/MgbEnemyCharacter.h"

AProjectile_Bananarang::AProjectile_Bananarang()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement->SetIsReplicated(true);
}

void AProjectile_Bananarang::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	SetLifeSpan(0.f);

	GetWorldTimerManager().SetTimer(
		OutBoundTimer,
		this,
		&AProjectile_Bananarang::ThrowBananarang,
		OutBoundFlightTime,
		false
	);
}


void AProjectile_Bananarang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bReturning)
	{
		AMgbWeapon* Weapon = Cast<AMgbWeapon>(GetOwner());
		AActor* PlayerActor = Weapon->GetOwner();
		float Distance = FVector::DistSquared(PlayerActor->GetActorLocation(),GetActorLocation());

		if (Distance <= 1600.f)
		{
			Destroy();
		}
	}
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
					FGameplayEffectSpecHandle EffectSpecHandle = Weapon->GetAbilitySystemComponent()->MakeOutgoingSpec(Weapon->DamageEffectClass, 1.f, EffectContextHandle);

					UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
					Weapon->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
				}
			}
		}
	}
	
}

void AProjectile_Bananarang::ThrowBananarang()
{
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->Deactivate();

	SetActorTickEnabled(true);

	FTimerHandle StayTimer;
	GetWorldTimerManager().SetTimer(
		StayTimer,
		this,
		&AProjectile_Bananarang::ReturnToPlayer,
		0.2f,
		false
	);
}

void AProjectile_Bananarang::ReturnToPlayer()
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
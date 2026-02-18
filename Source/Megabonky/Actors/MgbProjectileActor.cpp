// Copyright is owned by Veduy.


#include "MgbProjectileActor.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "../Core/Characters/MgbEnemyCharacter.h"
#include "../Core/MgbWeapon.h"
#include "../Core/MgbGameplayTags.h"
#include "../Util/NetworkLog.h"

// Sets default values
AMgbProjectileActor::AMgbProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
}

// Called when the game starts or when spawned
void AMgbProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(5.f);
}

// Called every frame
void AMgbProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMgbProjectileActor::OnCollisionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleOverlap(OtherActor);
}

void AMgbProjectileActor::HandleOverlap(AActor* OtherActor)
{
	if (!HasAuthority())
	{
		return;
	}

	// GetOwner() = WeaponActor;
	AMgbEnemyCharacter* Enemy = Cast<AMgbEnemyCharacter>(OtherActor);
	if (Enemy)
	{
		ActorsToIgnore.Add(Enemy);

		AMgbWeapon* Weapon = Cast<AMgbWeapon>(GetOwner());
		if (Weapon)
		{
			FGameplayEffectContextHandle EffectContextHandle = Weapon->GetAbilitySystemComponent()->MakeEffectContext();
			EffectContextHandle.AddSourceObject(Weapon);
			EffectContextHandle.AddInstigator(Weapon, Weapon);

			if (Weapon->DamageEffectClass)
			{
				FGameplayEffectSpecHandle EffectSpecHandle = Weapon->GetAbilitySystemComponent()->MakeOutgoingSpec(Weapon->DamageEffectClass, 1.f, EffectContextHandle);

				EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(TAG_Data_AreaDamageMultiplier, 1.f);
				UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
				Weapon->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);

				if (bBounce && BounceCount > 0)
				{
					Bounce();
					return;
				}
			}

			if (bRadialDamage == true)
			{
				float Radius = CollisionComp->GetScaledSphereRadius() * 5;

				TArray<AActor*> OutActors;

				UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), Radius,
					TArray<TEnumAsByte<EObjectTypeQuery>>(),
					AMgbEnemyCharacter::StaticClass(), 
					ActorsToIgnore,
					OutActors);

				for (const auto& Actor : OutActors)
				{
					if (!Actor)
						continue;

					float Length = (Actor->GetActorLocation() - GetActorLocation()).Length();
					float DistanceRatio = FMath::Clamp(1.f - (Length / Radius), 0.3f, 1.f);
					DistanceRatio = DistanceRatio;

					if (Weapon->DamageEffectClass)
					{
						FGameplayEffectSpecHandle EffectSpecHandle = Weapon->GetAbilitySystemComponent()->MakeOutgoingSpec(Weapon->DamageEffectClass, 1.f, EffectContextHandle);

						EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(TAG_Data_AreaDamageMultiplier, DistanceRatio);

						UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
						Weapon->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
					}
				}
				Destroy();
				return;
			}
			Destroy();
			return;
		}
		else
		{
			Destroy();
			return;
		}
	}
	
	Destroy();
}

void AMgbProjectileActor::Bounce()
{
	AActor* PrimaryTarget = nullptr;

	FVector Origin = GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<FHitResult> Hits;
	TArray<AActor*> OutActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Origin, BounceRange,
		ObjectTypes, AMgbEnemyCharacter::StaticClass(),
		ActorsToIgnore, OutActors);

	if (!OutActors.IsEmpty())
	{
		int32 Index = FMath::RandRange(0, OutActors.Num() - 1);
		PrimaryTarget = OutActors[Index];
	}
	else
	{
		Destroy();
	}

	if (PrimaryTarget)
	{
		FVector Direction = (PrimaryTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		float Speed = ProjectileMovement->Velocity.Size();
		ProjectileMovement->Velocity = Direction * Speed;
		SetActorRotation(Direction.Rotation());
	}

	BounceCount--;
}

void AMgbProjectileActor::Knockback()
{

}

void AMgbProjectileActor::HandleHitEffect_Implementation(AActor* TargetActor)
{

}

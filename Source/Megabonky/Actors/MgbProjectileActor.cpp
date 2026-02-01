// Copyright is owned by Veduy.


#include "MgbProjectileActor.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "../Core/Characters/MgbEnemyCharacter.h"
#include "../Core/MgbWeapon.h"
#include "../Util/NetworkLog.h"

// Sets default values
AMgbProjectileActor::AMgbProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

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

void AMgbProjectileActor::BeginOverlap(AActor* OtherActor)
{
	// 서버에서만 충돌 검사.
	if (!HasAuthority())
	{
		return;
	}

	// GetOwner() = WeaponActor;
	AMgbEnemyCharacter* Enemy = Cast<AMgbEnemyCharacter>(OtherActor);
	if (Enemy)
	{
		ActorsToIgnore.Add(Enemy);
		
		// 단일 대상 데미지 계산
		AMgbWeapon* Weapon = Cast<AMgbWeapon>(GetOwner());

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

				if (bBounce && BounceCount > 0)
				{
					Bounce();
					return;
				}
			}

			// 광역 데미지 계산
			if (bRadialDamage == true)
			{
				TArray<AActor*> OutActors;
				UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 200.f,
					TArray<TEnumAsByte<EObjectTypeQuery>>(),
					AMgbEnemyCharacter::StaticClass(),
					ActorsToIgnore, OutActors);

				// 일단 고정 딜.
				for (const auto& Actor : OutActors)
				{
					if (Weapon->DamageEffectClass)
					{
						// Spec을 생성한 컴포넌트가 ExecCalc의 Source로 설정.
						FGameplayEffectSpecHandle EffectSpecHandle = Weapon->GetAbilitySystemComponent()->MakeOutgoingSpec(Weapon->DamageEffectClass, 1.f, EffectContextHandle);

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
	else
	{
		Destroy();
	}
}

void AMgbProjectileActor::Bounce()
{
	// 범위내 랜덤 타겟 
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
		// 라이트닝 같은 능력의 바운스는 타겟만 지정시 발동되는 기술인데, 날라가는 투삭체가 아님.

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
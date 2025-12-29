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

		if (bRadialDamage == false)
		{	
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
					}
					else
					{
						Destroy();
					}
				}
			}
		}

		else if (bRadialDamage == true)
		{
			//static ENGINE_API bool ApplyRadialDamageWithFalloff(const UObject * WorldContextObject, float BaseDamage, float MinimumDamage, const FVector & Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>&IgnoreActors, AActor * DamageCauser = NULL, AController * InstigatedByController = NULL, ECollisionChannel DamagePreventionChannel = ECC_Visibility);
			//UGameplayStatics::ApplyRadialDamageWithFalloff();
		}
	}
}

void AMgbProjectileActor::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
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
		FVector Direction = (PrimaryTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		float Speed = ProjectileMovement->Velocity.Size();
		ProjectileMovement->Velocity = Direction * Speed;
		SetActorRotation(Direction.Rotation());
	}

	BounceCount--;
}

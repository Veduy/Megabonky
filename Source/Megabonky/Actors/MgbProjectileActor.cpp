// Copyright is owned by Veduy.


#include "MgbProjectileActor.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Core/Characters/MgbEnemyCharacter.h"

#include "../Util/NetworkLog.h"

// Sets default values
AMgbProjectileActor::AMgbProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->InitialSpeed = 300.f;
	ProjectileMovement->MaxSpeed = 300.f;
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

	// GetOwner() : WeaponActor;

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetOwner()->GetOwner());

	AMgbEnemyCharacter* Enemy = Cast<AMgbEnemyCharacter>(OtherActor);
	if (Enemy)
	{
		if (bRadialDamage == false)
		{	
			NET_LOG("ApplyDamage");
			//static ENGINE_API float ApplyDamage(AActor * DamagedActor, float BaseDamage, AController * EventInstigator, AActor * DamageCauser, TSubclassOf<class UDamageType> DamageTypeClass);
			UGameplayStatics::ApplyDamage(OtherActor, 1.f, PlayerController, GetOwner(), nullptr);
		}
		else if (bRadialDamage == true)
		{
			//static ENGINE_API bool ApplyRadialDamageWithFalloff(const UObject * WorldContextObject, float BaseDamage, float MinimumDamage, const FVector & Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>&IgnoreActors, AActor * DamageCauser = NULL, AController * InstigatedByController = NULL, ECollisionChannel DamagePreventionChannel = ECC_Visibility);
			//UGameplayStatics::ApplyRadialDamageWithFalloff();
		}
	}

	// OtherActor를 기준으로 

	//UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	//if (TargetASC)
	//{
	//	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	//	EffectContextHandle.AddSourceObject(this);
	//	EffectContextHandle.AddInstigator(GetOwner(), GetOwner()); 

	//	check(DamageEffectClass);
	//	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, 1.f, EffectContextHandle);
	//	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	//}
}

void AMgbProjectileActor::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}
	NET_LOG("");

}

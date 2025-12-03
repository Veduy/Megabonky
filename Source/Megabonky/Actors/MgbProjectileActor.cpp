// Copyright is owned by Veduy.


#include "MgbProjectileActor.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
AMgbProjectileActor::AMgbProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->InitialSpeed = 100.f;
	ProjectileMovement->MaxSpeed = 100.f;
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
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();


		TargetASC->ApplyGameplayEffectSpecToSelf()
	}
}
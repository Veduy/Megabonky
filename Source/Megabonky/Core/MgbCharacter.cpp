// Copyright is owned by Veduy.


#include "MgbCharacter.h"

#include "AbilitySystem/MgbAbilitySystemComponent.h"

// Sets default values
AMgbCharacter::AMgbCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UMgbAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

}

// Called when the game starts or when spawned
void AMgbCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMgbCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* AMgbCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float AMgbCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount > 0)
	{
		Destroy();
	}

	return 0.0f;
}


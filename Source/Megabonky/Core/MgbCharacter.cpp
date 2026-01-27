// Copyright is owned by Veduy.


#include "MgbCharacter.h"

#include "AbilitySystem/MgbAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "MgbWeapon.h"
#include "../Util/NetworkLog.h"

// Sets default values
AMgbCharacter::AMgbCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetNetUpdateFrequency(30.f);

	GetCharacterMovement()->bIgnoreBaseRotation = true;

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
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		
	//NET_LOG(FString::Printf(TEXT("DamageCauser : %s"), *DamageCauser->GetName()));			
	//NET_LOG(FString::Printf(TEXT("Weapon : %s"), *DamageCauser->GetName()));	
	//NET_LOG(FString::Printf(TEXT("EventInstigator : %s"), *EventInstigator->GetName()));	

	if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{	

	}

	return 0.0f;
}
// Copyright is owned by Veduy.


#include "MgbWeapon.h"

#include "../Core/AbilitySystem/MgbAbilitySystemComponent.h"
#include "../Core/AbilitySystem/AttributeSet/WeaponAttributeSet.h"

#include "../Util/NetworkLog.h"

AMgbWeapon::AMgbWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UMgbAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	WeaponAttributeSet = CreateDefaultSubobject<UWeaponAttributeSet>(TEXT("WeaponAttributeSet"));
}

void AMgbWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FGameplayAbilitySpec AbilitySpec = AbilitySystemComponent->BuildAbilitySpecFromClass(AbilityClass);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

UAbilitySystemComponent* AMgbWeapon::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
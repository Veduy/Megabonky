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

	ConstructorHelpers::FClassFinder<UGameplayEffect>GE_DamageEffect(TEXT("/Game/Blueprints/AbilitySystem/GameplayEffects/GE_DamageEffect.GE_DamageEffect_C"));
	if (GE_DamageEffect.Succeeded())
	{
		DamageEffectClass = GE_DamageEffect.Class;
	}

}

void AMgbWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && AbilityClass)
	{
		FGameplayAbilitySpec AbilitySpec = AbilitySystemComponent->BuildAbilitySpecFromClass(AbilityClass);
		AbilitySystemComponent->GiveAbility(AbilitySpec);

		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InitAttributeEffectClass, 1.f, ContextHandle);
		
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

UAbilitySystemComponent* AMgbWeapon::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
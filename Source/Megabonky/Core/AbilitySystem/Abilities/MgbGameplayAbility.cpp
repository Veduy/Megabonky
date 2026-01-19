// Copyright is owned by Veduy.


#include "MgbGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UMgbGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UMgbGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

UAbilitySystemComponent* UMgbGameplayAbility::GetPlayerASC()
{
	AActor* PlayerActor = GetActorInfo().AvatarActor.Get()->GetOwner(); // PlayerActor
	UAbilitySystemComponent* PlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerActor);
	
	return PlayerASC;
}

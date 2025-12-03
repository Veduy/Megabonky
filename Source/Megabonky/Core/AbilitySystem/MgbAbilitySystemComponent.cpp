// Copyright is owned by Veduy.


#include "MgbAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"

void UMgbAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

void UMgbAbilitySystemComponent::OnPlayerControllerSet()
{
	Super::OnPlayerControllerSet();
}

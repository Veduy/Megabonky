// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MgbAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKY_API UMgbAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/**
	 *	Initialized the Abilities' ActorInfo - the structure that holds information about who we are acting on and who controls us.
	 *      OwnerActor is the actor that logically owns this component.
	 *		AvatarActor is what physical actor in the world we are acting on. Usually a Pawn but it could be a Tower, Building, Turret, etc, may be the same as Owner
	 */
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	

	/** called when the ASC's AbilityActorInfo has a PlayerController set. */
	virtual void OnPlayerControllerSet() override;
};

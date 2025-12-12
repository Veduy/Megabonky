// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MgbGameplayAbility.generated.h"


// ----------------------------------------------------------------------------------------------------------------
//
//	The important functions:
//	
//		CanActivateAbility()	- const function to see if ability is activatable. Callable by UI etc
//
//		TryActivateAbility()	- Attempts to activate the ability. Calls CanActivateAbility(). Input events can call this directly.
//								- Also handles instancing-per-execution logic and replication/prediction calls.
//		
//		CallActivateAbility()	- Protected, non virtual function. Does some boilerplate 'pre activate' stuff, then calls ActivateAbility()
//
//		ActivateAbility()		- What the abilities *does*. This is what child classes want to override.
//	
//		CommitAbility()			- Commits reources/cooldowns etc. ActivateAbility() must call this!
//		
//		CancelAbility()			- Interrupts the ability (from an outside source).
//
//		EndAbility()			- The ability has ended. This is intended to be called by the ability to end itself.
//	
// ----------------------------------------------------------------------------------------------------------------

/**
 * 
 */

UCLASS(Blueprintable)
class MEGABONKY_API UMgbGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	/** Called when the ability is given to an AbilitySystemComponent */
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};

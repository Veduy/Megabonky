// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MgbEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class MEGABONKY_API AMgbEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMgbEffectActor();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);

};

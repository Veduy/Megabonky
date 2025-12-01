// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MgbCharacter.generated.h"

class UMgbAbilitySystemComponent;
class UMgbAttributeSet;

UCLASS()
class MEGABONKY_API AMgbCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMgbCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UMgbAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	const TObjectPtr<UMgbAttributeSet> AttributeSet;

};

// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbCharacter.h"
#include "MgbEnemyCharacter.generated.h"


class UCharacterAttributeSet;
/**
 * 
 */
UCLASS()
class MEGABONKY_API AMgbEnemyCharacter : public AMgbCharacter
{
	GENERATED_BODY()

public:
	AMgbEnemyCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<UCharacterAttributeSet> CharacterAttributeSet;
};

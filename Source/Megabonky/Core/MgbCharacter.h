// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MgbCharacter.generated.h"

class UMgbAbilitySystemComponent;

UCLASS()
class MEGABONKY_API AMgbCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMgbCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY()
	TObjectPtr<UMgbAbilitySystemComponent> ASC;

};

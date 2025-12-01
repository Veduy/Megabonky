// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MgbWeapon.generated.h"

UCLASS()
class MEGABONKY_API AMgbWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AMgbWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

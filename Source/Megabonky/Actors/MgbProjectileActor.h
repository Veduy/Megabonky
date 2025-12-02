// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MgbProjectileActor.generated.h"

class UProjectileMovementComponent;

UCLASS()
class MEGABONKY_API AMgbProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMgbProjectileActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
};

// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbProjectileActor.h"
#include "Projectile_Bananarang.generated.h"

/**
 *
 */
UCLASS()
class MEGABONKY_API AProjectile_Bananarang : public AMgbProjectileActor
{
	GENERATED_BODY()
	
public:
	AProjectile_Bananarang();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	virtual void BeginOverlap(AActor* OtherActor);

public:
	UFUNCTION()
	void OutBoundFlight();

	UFUNCTION()
	void ReturnToPlayer();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	float OutBoundFlightTime = 0.7f;

	UPROPERTY()
	uint8 bReturning : 1 = false;

	UPROPERTY()
	FTimerHandle OutBoundTimer;
};

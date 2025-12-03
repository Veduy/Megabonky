// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MgbProjectileActor.generated.h"

class UProjectileMovementComponent;
class UPrimitiveComponent;

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
	/** Delegate for notification of start of overlap with a specific component */
	//FComponentBeginOverlapSignature, UPrimitiveComponent, OnComponentBeginOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, 
	//	const FHitResult&, SweepResult);

	UFUNCTION(BlueprintCallable)
	void BeginOverlap(AActor* OtherActor);
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
};

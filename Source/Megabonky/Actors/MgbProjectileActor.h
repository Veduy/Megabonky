// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MgbProjectileActor.generated.h"

class UProjectileMovementComponent;
class UPrimitiveComponent;
class UGameplayEffect;

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
	//FComponentBeginOverlapSignature, UPrimitiveComponent, OnComponentBeginOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult&, SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void BeginOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION(BlueprintCallable)
	void Bounce();

	UFUNCTION(BlueprintCallable)
	void Knockback();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<TObjectPtr<AActor>> ActorsToIgnore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	uint8 bRadialDamage : 1 = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	uint8 bBounce : 1 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 BounceCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	float BounceRange = 500.f;
};

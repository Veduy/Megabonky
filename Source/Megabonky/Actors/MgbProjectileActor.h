// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "MgbProjectileActor.generated.h"

class USphereComponent;
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
	UFUNCTION(BlueprintCallable)
	virtual void BeginOverlap(AActor* OtherActor);

public:
	UFUNCTION(BlueprintCallable)
	void Bounce();

	UFUNCTION(BlueprintCallable)
	void Knockback();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HandleHitEffect(AActor* TargetActor);
	void HandleHitEffect_Implementation(AActor* TargetActor);

public:
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<TObjectPtr<AActor>> ActorsToIgnore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	FGameplayTag HitEffectCueTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	uint8 bRadialDamage : 1 = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	uint8 bBounce : 1 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 BounceCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	float BounceRange = 500.f;
};

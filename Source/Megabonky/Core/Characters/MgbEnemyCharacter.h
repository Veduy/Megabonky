// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbCharacter.h"
#include "AbilitySystemInterface.h"
#include "MgbEnemyCharacter.generated.h"

class UEnemyAttributeSet;
class UCapsuleComponent;
class UFloatingPawnMovement;
class USkeletalMeshComponent;
class UGameplayEffect;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Idle		= 0		UMETA(DisplayName = "Idle"),
	Walk		= 5		UMETA(DisplayName = "Walk"),
	Climb		= 10	UMETA(DisplayName = "Climb")
};

UCLASS()
class MEGABONKY_API AMgbEnemyCharacter : public AMgbCharacter
{
	GENERATED_BODY()

public:
	AMgbEnemyCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Destroyed() override;

public:
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void LookTarget(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void CheckWall();

	UFUNCTION(BlueprintCallable)
	void ClimbWall(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void CollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HandleDamageEffect();
	void HandleDamageEffect_Implementation();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TSubclassOf<UGameplayEffect> HitDamageEffectClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UEnemyAttributeSet> EnemyAttributeSet;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	EMoveState CurrentMoveState = EMoveState::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 bSpawnFinished : 1 = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TSubclassOf<AActor> XPCrystalClass;

	UPROPERTY()
	float TargetSpawnHeight;

	FTimerHandle CheckWallTimer;
};
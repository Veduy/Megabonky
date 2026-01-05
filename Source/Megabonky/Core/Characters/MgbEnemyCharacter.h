// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "MgbEnemyCharacter.generated.h"

class UEnemyAttributeSet;
class UCapsuleComponent;
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
class MEGABONKY_API AMgbEnemyCharacter : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMgbEnemyCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Destroyed() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	UFUNCTION(BlueprintCallable)
	UCapsuleComponent* GetCapsuleComponent();

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetMesh();

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

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	//FComponentHitSignature OnComponentHit;

	//FComponentHitSignature, UPrimitiveComponent, OnComponentHit, UPrimitiveComponent*, HitComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, FVector, NormalImpulse, const FHitResult&, Hit );
	



public:
	// 초기 Attribute 값 적용할 GameplayEffect.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TSubclassOf<UGameplayEffect> InitAttributeEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UEnemyAttributeSet> CharacterAttributeSet;

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
};

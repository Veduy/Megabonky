// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbCharacter.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"

#include "MgbPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
class UInputAction;
class UGameplayAbility;
class AMgbWeapon;

USTRUCT(BlueprintType)
struct FTomes
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGameplayTag Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Level;
};

/**
 * 
 */
UCLASS() 
class MEGABONKY_API AMgbPlayerCharacter : public AMgbCharacter
{
	GENERATED_BODY()

public:
	AMgbPlayerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable)
	bool FindPrimaryTargetByCondition(AActor*& OutPrimaryTarget);

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(TSubclassOf<AMgbWeapon> NewWeapon);

	UFUNCTION(BlueprintCallable)
	void ActivateWeaponsAbility();

	void UpdateCharacterMeshRotation(float DeltaTime);

	//UPrimitiveComponent, OnComponentBeginOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult&, SweepResult)

	UFUNCTION(BlueprintCallable)
	void PickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> MainCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> PickupSphere;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<AMgbWeapon> DefaultWeaponClass;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<TObjectPtr<AMgbWeapon>> Weapons;

	/// 비전서 (Tomes)
	UPROPERTY(Replicated, VisibleAnywhere,BlueprintReadWrite, Category = "Data")
	TArray<FTomes> Tomes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAttributeSet> PlayerAttributeSet;

	FTimerHandle ActivateAbilityHandle;
	
	FQuat MeshBaseQuat;

public:
	UFUNCTION()
	virtual void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta=(DisplayName = "IA_Move"))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta=(DisplayName = "IA_Look"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta=(DisplayName = "IA_Jump"))
	TObjectPtr<UInputAction> JumpAction;
};

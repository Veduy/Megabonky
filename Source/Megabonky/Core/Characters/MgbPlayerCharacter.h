// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbCharacter.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"

#include "MgbPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
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

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable)
	bool FindPrimaryTargetByCondition(AActor*& OutPrimaryTarget);

	UFUNCTION()
	void SpawnDefaultWeapon();

	UFUNCTION()
	void ActivateWeaponsAbility();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> MainCamera;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<AMgbWeapon> DefaultWeaponClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Data")
	TArray<TObjectPtr<AMgbWeapon>> Weapons;

	/// 비전서 (Tomes)
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Data", Replicated)
	TArray<FTomes> Tomes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAttributeSet> PlayerAttributeSet;

	FTimerHandle ActivateAbilityHandle;

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

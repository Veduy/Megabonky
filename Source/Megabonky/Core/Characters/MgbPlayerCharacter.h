// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbCharacter.h"
#include "GameplayAbilitySpec.h"

#include "MgbPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UGameplayAbility;
class AMgbWeapon;

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
	UFUNCTION()
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TSubclassOf<AMgbWeapon> DefaultWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TArray<TObjectPtr<AMgbWeapon>> Weapons;

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

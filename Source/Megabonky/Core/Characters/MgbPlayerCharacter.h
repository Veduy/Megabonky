// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbCharacter.h"
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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> MainCamera;

public:
	/** Gameplay Ability System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> TestAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	TArray<TObjectPtr<AMgbWeapon>> Weapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<UAttributeSet> CharacterAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<UAttributeSet> PlayerAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<UAttributeSet> WeaponAttributeSet;

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

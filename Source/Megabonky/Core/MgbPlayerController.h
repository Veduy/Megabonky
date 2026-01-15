// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/WeaponInfo.h"
#include "MgbPlayerController.generated.h"

class UInputMappingContext;
class UInGame;
class UGameplayEffect;
class UDataTable;

/**
 * 
 */
UCLASS()
class MEGABONKY_API AMgbPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMgbPlayerController();

	/** Called on the client to do local pawn setup after possession, before calling ServerAcknowledgePossession */
	virtual void AcknowledgePossession(class APawn* P) override;

	virtual void BeginPlay() override;
protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

public:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerResumeRequestCountIncrementAndCheck();
	void ServerResumeRequestCountIncrementAndCheck_Implementation();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerApplyWeaponUpgradeEffect(FName InWeaponName, const TArray<FWeaponUpgradeOption>& UpgradeData);
	void ServerApplyWeaponUpgradeEffect_Implementation(FName InWeaponName, const TArray<FWeaponUpgradeOption>& UpgradeData);

public:
	UFUNCTION(Client, Unreliable)
	void ClientSpawnDamageTextActor(FVector Location, float DamageValue);
	void ClientSpawnDamageTextActor_Implementation(FVector Location, float DamageValue);

	UFUNCTION(BlueprintCallable)
	void GenerateUpgradeInfo();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
	TObjectPtr<UInGame> InGameWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TSubclassOf<AActor> DamageTextActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TSubclassOf<UGameplayEffect> GE_WeaponUpgradeDefaultClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UDataTable> DT_Weapons;
};

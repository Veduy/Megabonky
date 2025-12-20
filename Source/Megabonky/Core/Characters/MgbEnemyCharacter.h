// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbCharacter.h"
#include "MgbEnemyCharacter.generated.h"


class UEnemyAttributeSet;
/**
 * 
 */
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
	UFUNCTION()
	void MoveToTarget();

	UFUNCTION()
	void LookTarget();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UEnemyAttributeSet> CharacterAttributeSet;

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 bSpawnFinished : 1 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<AActor> XPCrystalClass;

	UPROPERTY()
	float TargetSpawnHeight;
};

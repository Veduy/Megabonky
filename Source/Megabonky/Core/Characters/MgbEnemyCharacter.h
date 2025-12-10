// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "../MgbCharacter.h"
#include "MgbEnemyCharacter.generated.h"


class UCharacterAttributeSet;
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

public:
	UFUNCTION()
	void MoveToTarget();

	UFUNCTION()
	void LookTarget();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UCharacterAttributeSet> CharacterAttributeSet;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 bSpawnFinished : 1 = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<AActor> TargetActor;

	UPROPERTY()
	float TargetSpawnHeight;
};

// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MgbPlayerController.generated.h"

class UInputMappingContext;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;


};

// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MgbPlayerController.generated.h"

class UInputMappingContext;
class UInGame;

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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
	TObjectPtr<UInGame> InGameWidget;
};

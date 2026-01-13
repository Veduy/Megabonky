// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MgbItemActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAction, AActor*, TargetActor);

UCLASS()
class MEGABONKY_API AMgbItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMgbItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FOnItemAction OnAction;
};

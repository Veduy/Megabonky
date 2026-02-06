// Copyright is owned by Veduy.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "MgbEditorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MEGABONKY_API UMgbEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec)
	void TestCmd();
};

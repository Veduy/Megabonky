// Copyright is owned by Veduy.


#include "MgbPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AMgbPlayerController::AMgbPlayerController()
{
}

void AMgbPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

}

void AMgbPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AMgbPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

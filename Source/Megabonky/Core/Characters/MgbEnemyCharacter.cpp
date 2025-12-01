// Copyright is owned by Veduy.


#include "MgbEnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "../AbilitySystem/MgbAttributeSet.h"

AMgbEnemyCharacter::AMgbEnemyCharacter()
{
}

void AMgbEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->GetSet<UMgbAttributeSet>();
	}
}

void AMgbEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

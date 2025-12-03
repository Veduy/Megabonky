// Copyright is owned by Veduy.


#include "MgbEnemyCharacter.h"

#include "../AbilitySystem/AttributeSet/CharacterAttributeSet.h"

AMgbEnemyCharacter::AMgbEnemyCharacter()
{
	CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
}

void AMgbEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AMgbEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

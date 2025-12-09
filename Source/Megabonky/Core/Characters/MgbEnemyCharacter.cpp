// Copyright is owned by Veduy.


#include "MgbEnemyCharacter.h"

#include "../AbilitySystem/AttributeSet/CharacterAttributeSet.h"
#include "Components/CapsuleComponent.h"

AMgbEnemyCharacter::AMgbEnemyCharacter()
{
	CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
}

void AMgbEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	TargetSpawnHeight = GetActorLocation().Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f;
}

void AMgbEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bSpawnFinished)
	{
		float Height = FMath::FInterpTo(GetActorLocation().Z, TargetSpawnHeight, DeltaTime, 2.f);
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, Height));
		
		if (GetActorLocation().Z >= TargetSpawnHeight)
		{
			bSpawnFinished = true;
		}
	}
}

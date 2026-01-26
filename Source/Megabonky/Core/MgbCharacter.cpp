// Copyright is owned by Veduy.


#include "MgbCharacter.h"

#include "AbilitySystem/MgbAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "MgbWeapon.h"
#include "../Util/NetworkLog.h"

// Sets default values
AMgbCharacter::AMgbCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetNetUpdateFrequency(30.f);

	GetCharacterMovement()->bIgnoreBaseRotation = true;

	AbilitySystemComponent = CreateDefaultSubobject<UMgbAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

// Called when the game starts or when spawned
void AMgbCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 애니메이션 뚝뚝끊기게 테스트.
	auto MeshComp = GetMesh();
	if (MeshComp)
	{
		MeshComp->bEnableUpdateRateOptimizations = true;

		// 보간 비활성
		MeshComp->AnimUpdateRateParams->bInterpolateSkippedFrames = false;

		MeshComp->AnimUpdateRateParams->bSkipUpdate = true;
		MeshComp->AnimUpdateRateParams->bSkipEvaluation = true;
		MeshComp->AnimUpdateRateParams->bShouldUseLodMap = true;

		// Reset and add frame skips. 
		// A skip of 10 means it ticks every 11th frame (~5.4 FPS at 60 FPS).
		MeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Empty();
		MeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Add(0, 15);
		MeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Add(1, 15);
		MeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Add(2, 15);
		MeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Add(3, 15);

		MeshComp->AnimUpdateRateParams->BaseNonRenderedUpdateRate = 2; // Very low rate when not visible

		MeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
		MeshComp->SetForcedLOD(0);
	}
}

// Called every frame
void AMgbCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* AMgbCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float AMgbCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		
	//NET_LOG(FString::Printf(TEXT("DamageCauser : %s"), *DamageCauser->GetName()));			
	//NET_LOG(FString::Printf(TEXT("Weapon : %s"), *DamageCauser->GetName()));	
	//NET_LOG(FString::Printf(TEXT("EventInstigator : %s"), *EventInstigator->GetName()));	

	if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{	

	}

	return 0.0f;
}
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

	// “LOD에 따라 애니메이션 평가 빈도를 조절하는 Unreal의 Update Rate Optimization 시스템 사용 모든 LOD에서 애니메이션 프레임 스킵을 적용.
	USkeletalMeshComponent* MeshComp = GetMesh();
	MeshComp->bEnableUpdateRateOptimizations = true;
	MeshComp->AnimUpdateRateParams->bShouldUseLodMap = true;
	MeshComp->AnimUpdateRateParams->MaxEvalRateForInterpolation = 0;
	MeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Add(0, 10);
	MeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Add(1, 10);
	MeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Add(2, 10);
	MeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Add(3, 10);
	MeshComp->AnimUpdateRateParams->BaseNonRenderedUpdateRate = 6;

	MeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;

	MeshComp->SetForcedLOD(0);
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
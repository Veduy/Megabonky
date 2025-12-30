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

	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UMgbAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

// Called when the game starts or when spawned
void AMgbCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 애니메이션 테스트중.
	USkeletalMeshComponent* MeshComp = GetMesh();
	MeshComp->bEnableUpdateRateOptimizations = true;
	MeshComp->AnimUpdateRateParams->bShouldUseLodMap = true;
	MeshComp->SetForcedLOD(2);
	MeshComp->AnimUpdateRateParams->MaxEvalRateForInterpolation = 0;
	MeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Add(0, 10);
	MeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Add(1, 10);
	MeshComp->AnimUpdateRateParams->LODToFrameSkipMap.Add(2, 10);
	MeshComp->AnimUpdateRateParams->BaseNonRenderedUpdateRate = 6;

	//MeshComp->AnimUpdateRateParams->bShouldUseLodMap = false;
	//MeshComp->AnimUpdateRateParams->MaxEvalRateForInterpolation = 0;
	//MeshComp->AnimUpdateRateParams->UpdateRate = 10;
	MeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
}

// Called every frame
void AMgbCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCharacterPitch(DeltaTime);
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

void AMgbCharacter::UpdateCharacterPitch(float DeltaTime)
{
	//const FFindFloorResult& Floor = GetCharacterMovement()->CurrentFloor;

	//if (Floor.bBlockingHit)
	//{
	//	FVector FloorNormal = Floor.HitResult.Normal;

	//	FRotator RotationXZ = UKismetMathLibrary::MakeRotFromXZ(GetActorForwardVector(),FloorNormal);
	//	FRotator RotationYZ = UKismetMathLibrary::MakeRotFromYZ(GetActorRightVector(),FloorNormal);
	//	
	//	FRotator CurrentRotation = GetMesh()->GetRelativeRotation();

	//	FRotator TargetRotation;
	//	TargetRotation.Pitch = RotationYZ.Pitch;
	//	TargetRotation.Roll = RotationXZ.Roll;
	//	
	//	// Mesh가 기본으로 Yaw축으로 -90도 돌아가있어서 > 
	//	CurrentRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetRotation.Roll, DeltaTime, 5.f);
	//	CurrentRotation.Roll = FMath::FInterpTo(CurrentRotation.Roll, -TargetRotation.Pitch, DeltaTime, 5.f);

	//	GetMesh()->SetRelativeRotation(CurrentRotation);
	//}
	//else
	//{
	//	FRotator CurrentRotation = GetMesh()->GetRelativeRotation();
	//	
	//	CurrentRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, 0, DeltaTime, 5.f);
	//	CurrentRotation.Roll = FMath::FInterpTo(CurrentRotation.Roll, 0, DeltaTime, 5.f);

	//	GetMesh()->SetRelativeRotation(CurrentRotation);
	//}

	FVector Start = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	FVector End = Start + (GetActorUpVector() * -500.f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;

	bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		Start, End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
		ActorsToIgnore, EDrawDebugTrace::Type::ForOneFrame, Hit,
		true);

	if (bResult)
	{
		FVector FloorNormal = Hit.Normal;

		FRotator RotationXZ = UKismetMathLibrary::MakeRotFromXZ(GetActorForwardVector(), FloorNormal);
		FRotator RotationYZ = UKismetMathLibrary::MakeRotFromYZ(GetActorRightVector(), FloorNormal);

		FRotator CurrentRotation = GetMesh()->GetRelativeRotation();

		FRotator TargetRotation;
		TargetRotation.Pitch = RotationYZ.Pitch;
		TargetRotation.Roll = RotationXZ.Roll;

		// Mesh가 기본으로 Yaw축으로 -90도 돌아가있어서 
		CurrentRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetRotation.Roll, DeltaTime, 5.f);
		CurrentRotation.Roll = FMath::FInterpTo(CurrentRotation.Roll, -TargetRotation.Pitch, DeltaTime, 5.f);

		GetMesh()->SetRelativeRotation(CurrentRotation);
	}
	else
	{
		FRotator CurrentRotation = GetMesh()->GetRelativeRotation();

		CurrentRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, 0, DeltaTime, 5.f);
		CurrentRotation.Roll = FMath::FInterpTo(CurrentRotation.Roll, 0, DeltaTime, 5.f);

		GetMesh()->SetRelativeRotation(CurrentRotation);
	}
}

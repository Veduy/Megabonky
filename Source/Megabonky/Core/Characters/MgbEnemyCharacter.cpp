// Copyright is owned by Veduy.


#include "MgbEnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "../AbilitySystem/AttributeSet/CharacterAttributeSet.h"
#include "../../Util/NetworkLog.h"

AMgbEnemyCharacter::AMgbEnemyCharacter()
{
	CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));

	// 스폰시 AIController가 있으면 유효한 위치로 강제 이동시킴.
	// 스폰로직이 끝났을때, SpawnDefaultController(); 로 AIController 붙여줌.
	AutoPossessAI = EAutoPossessAI::Disabled;

	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MaxAcceleration = 500.f;
	GetCharacterMovement()->GroundFriction = 100.f;
	GetCharacterMovement()->MaxStepHeight = 1000.f;
}

void AMgbEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	TargetSpawnHeight = GetActorLocation().Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);
	}
}

void AMgbEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookTarget();

	if (bSpawnFinished == false)
	{
		float Height = FMath::FInterpTo(GetActorLocation().Z, TargetSpawnHeight, DeltaTime, 1.5f);
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, Height));
		
		if (GetActorLocation().Z >= TargetSpawnHeight - 5.f)
		{
			bSpawnFinished = true;   
			SpawnDefaultController();
		}
	}

	if (bSpawnFinished == true)
	{
		MoveToTarget();
	}
}

void AMgbEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMgbEnemyCharacter, TargetActor);
}

void AMgbEnemyCharacter::MoveToTarget()
{
	if (!TargetActor)
	{
		return;
	}

	FVector Dir = TargetActor->GetActorLocation() - GetActorLocation();
	Dir = Dir.GetSafeNormal2D();
	AddMovementInput(Dir, 1.f);

	//bool UKismetSystemLibrary::LineTraceSingleForObjects(const UObject * WorldContextObject, const FVector Start, const FVector End, const TArray<TEnumAsByte<EObjectTypeQuery> > &ObjectTypes, bool bTraceComplex, const TArray<AActor*>&ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult & OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)

	FVector Start = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	FVector End = Start + GetActorForwardVector() * (GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.5f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;

	bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		Hit,
		true);

	// 타겟과 벽을 사이에 두고있을 경우. 벽을 타고 올라가도록.
	if (bResult)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		AddMovementInput(GetActorUpVector());
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

		FVector GroundCheckStart = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector GroundCheckCheckEnd = GroundCheckStart + GetActorUpVector() * (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * -1.2f);
		TArray<TEnumAsByte<EObjectTypeQuery>> GroundCheckObjectTypes;
		GroundCheckObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

		TArray<AActor*> GroundCheckActorsToIgnore;
		FHitResult GroundCheckHit;

		bool bGroundCheckResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
			GroundCheckStart,
			GroundCheckCheckEnd,
			GroundCheckObjectTypes,
			false,
			GroundCheckActorsToIgnore,
			EDrawDebugTrace::ForOneFrame,
			GroundCheckHit,
			true);

		if (bGroundCheckResult)
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
		else
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		}
	}
}

void AMgbEnemyCharacter::LookTarget()
{
	if (TargetActor)
	{
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation());
		FRotator LookRotation = FRotator(GetActorRotation().Pitch, TargetRotation.Yaw, GetActorRotation().Roll);
		SetActorRotation(LookRotation);
	}
}

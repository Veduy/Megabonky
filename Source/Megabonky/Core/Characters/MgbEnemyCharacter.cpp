// Copyright is owned by Veduy.

#include "MgbEnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "../AbilitySystem/MgbAbilitySystemComponent.h"
#include "../AbilitySystem/AttributeSet/EnemyAttributeSet.h"
#include "../../Util/NetworkLog.h"

AMgbEnemyCharacter::AMgbEnemyCharacter()
{
	bReplicates = true;

	AutoPossessAI = EAutoPossessAI::Disabled;

	CharacterAttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("EnemyAttributeSet"));

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetCollisionProfileName(FName("Enemy"));
	CapsuleComponent->SetCapsuleHalfHeight(80.f);
	CapsuleComponent->SetCapsuleRadius(40.f);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);
	SkeletalMeshComponent->SetCollisionProfileName("NoCollision");
	SkeletalMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.f, 0.0f));
	float Half = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	SkeletalMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -Half));
	SkeletalMeshComponent->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));

	AbilitySystemComponent = CreateDefaultSubobject<UMgbAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	//GetCharacterMovement()->MaxWalkSpeed = 100.f;
	//GetCharacterMovement()->MaxAcceleration = 500.f;
	//GetCharacterMovement()->GroundFriction = 1.f;
	//GetCharacterMovement()->MaxStepHeight = 1000.f;
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

	// WallChecking Timer




}

void AMgbEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	if (bSpawnFinished)
	{
		switch (CurrentMoveState)
		{
		case EMoveState::Idle:
			LookTarget();
			break;
		case EMoveState::Walk:
			LookTarget();
			MoveToTarget(DeltaTime);
			break;
		case EMoveState::Climb:
			LookTarget();
			ClimbWall(DeltaTime);
			break;
		default:
			break;
		}
	}
}

void AMgbEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMgbEnemyCharacter, TargetActor);
}

void AMgbEnemyCharacter::Destroyed()
{
	//서버일때만 XPCrystal Drop
	if (HasAuthority())
	{
		//GetWorld()->SpawnActor<AActor>(XPCrystalClass, GetActorTransform());
	}
}

UAbilitySystemComponent* AMgbEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCapsuleComponent* AMgbEnemyCharacter::GetCapsuleComponent()
{
	return CapsuleComponent;
}

USkeletalMeshComponent* AMgbEnemyCharacter::GetMesh()
{
	return SkeletalMeshComponent;
}

void AMgbEnemyCharacter::MoveToTarget(float DeltaTime)
{
	if(!TargetActor)
	{
		return;
	}

	FVector Dir = TargetActor->GetActorLocation() - GetActorLocation();
	Dir = Dir.GetSafeNormal2D();
	Dir.Z = 0;
	AddActorWorldOffset(Dir * 200.f * DeltaTime);	
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

void AMgbEnemyCharacter::CheckWall()
{
	FVector Dir = TargetActor->GetActorLocation() - GetActorLocation();
	Dir = Dir.GetSafeNormal();
	FVector Start = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	FVector End = Start + Dir * (GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.0f);
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

	if (bResult)
	{
		// 타겟과 벽을 사이에 두고있을 경우. 벽을 타고 올라가도록.
		FVector Normal = Hit.ImpactNormal;
		FVector Forward = GetActorForwardVector();
		float value = FVector::DotProduct(Normal, Forward);
		if (value <= -0.9f)
		{
			GetCapsuleComponent()->SetEnableGravity(false);
			CurrentMoveState = EMoveState::Climb;
		}
	}
	else
	{
		GetCapsuleComponent()->SetEnableGravity(true);
		CurrentMoveState = EMoveState::Walk;
	}
	/*else
	{
		FVector GroundCheckStart = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector GroundCheckCheckEnd = GroundCheckStart + GetActorUpVector() * (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * -0.5f);
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
			EDrawDebugTrace::None,
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
	}*/
}

void AMgbEnemyCharacter::ClimbWall(float DeltaTime)
{
	FVector Dir = GetActorUpVector();
	AddActorWorldOffset(Dir * 200 * DeltaTime);
}

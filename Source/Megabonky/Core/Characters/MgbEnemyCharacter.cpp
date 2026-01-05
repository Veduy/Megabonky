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

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->SetCollisionProfileName(FName("Enemy"));
	CapsuleComponent->SetCapsuleHalfHeight(80.f);
	CapsuleComponent->SetCapsuleRadius(40.f);
	CapsuleComponent->SetSimulatePhysics(false);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetCollisionProfileName("NoCollision");
	SkeletalMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.f, 0.0f));
	float Half = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	SkeletalMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -Half));
	SkeletalMeshComponent->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));

	AbilitySystemComponent = CreateDefaultSubobject<UMgbAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CharacterAttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("EnemyAttributeSet"));

	AutoPossessAI = EAutoPossessAI::Disabled;
}

void AMgbEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//서버일때만
	if (!HasAuthority())
		return;

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

	if (bSpawnFinished == false)
	{
		float Height = FMath::FInterpTo(GetActorLocation().Z, TargetSpawnHeight, DeltaTime, 1.5f);
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, Height));
		
		if (GetActorLocation().Z >= TargetSpawnHeight - 5.f)
		{
			bSpawnFinished = true;   
			CapsuleComponent->SetSimulatePhysics(true);
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
	//서버일때만
	if (!HasAuthority())
		return;

	if(!TargetActor)
	{
		return;
	}

	//경사면에서 일정한 속도값을 위해서.
	FVector Start = GetActorLocation();
	FVector End = Start + (GetActorUpVector() * CapsuleComponent->GetScaledCapsuleHalfHeight() * -1.3f);
	FHitResult Hit;

	// 추후에 디버깅할때 비용 확인할수 있음.
	FCollisionQueryParams Params(SCENE_QUERY_STAT(GroundTrace), false);

	bool bResult = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_WorldStatic,
		Params
	);

	if (bResult)
	{
		FVector Dir = TargetActor->GetActorLocation() - GetActorLocation();
		Dir = Dir.GetSafeNormal2D();

		Dir = -Hit.ImpactNormal + Dir;
		AddActorWorldOffset(Dir * 150.f * DeltaTime);
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

void AMgbEnemyCharacter::CheckWall()
{
	FVector Dir = TargetActor->GetActorLocation() - GetActorLocation();
	Dir = Dir.GetSafeNormal2D();
	FVector Start = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.9f);
	FVector End = Start + Dir * (GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.5f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult Hit;

	bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hit,
		true);

	if (bResult && CurrentMoveState != EMoveState::Climb)
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
		else if (Cast<AMgbEnemyCharacter>(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy Climb"));
			GetCapsuleComponent()->SetEnableGravity(false);
			CurrentMoveState = EMoveState::Climb;
		}
	}
	else if(!bResult && CurrentMoveState != EMoveState::Walk)
	{
		GetCapsuleComponent()->SetEnableGravity(true);
		CurrentMoveState = EMoveState::Walk;
	}
}

void AMgbEnemyCharacter::ClimbWall(float DeltaTime)
{
	FVector Dir = TargetActor->GetActorLocation() - GetActorLocation();
	Dir = Dir.GetSafeNormal();
	AddActorWorldOffset(Dir * 150 * DeltaTime);
}

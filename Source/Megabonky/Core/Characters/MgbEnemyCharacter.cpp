// Copyright is owned by Veduy.


#include "MgbEnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "../AbilitySystem/AttributeSet/EnemyAttributeSet.h"
#include "../../Util/NetworkLog.h"

AMgbEnemyCharacter::AMgbEnemyCharacter()
{
	bReplicates = true;

	CharacterAttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("EnemyAttributeSet"));
	
	AutoPossessAI = EAutoPossessAI::Disabled;

	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	GetCharacterMovement()->MaxAcceleration = 500.f;
	GetCharacterMovement()->GroundFriction = 1.f;
	GetCharacterMovement()->MaxStepHeight = 1000.f;

	GetCapsuleComponent()->SetCollisionProfileName(FName("Enemy"));
	GetCapsuleComponent()->SetCapsuleHalfHeight(80.f);
	GetCapsuleComponent()->SetCapsuleRadius(40.f);

	GetMesh()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.f, 0.0f));
	float Half = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -Half));
	GetMesh()->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));
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

void AMgbEnemyCharacter::Destroyed()
{
	//서버일때만 XPCrystal Drop
	if (HasAuthority())
	{
		GetWorld()->SpawnActor<AActor>(XPCrystalClass, GetActorTransform());
	}
}

void AMgbEnemyCharacter::MoveToTarget()
{
	if (!TargetActor)
	{
		return;
	}

	FVector Dir = TargetActor->GetActorLocation() - GetActorLocation();
	Dir = Dir.GetSafeNormal2D();
	AddMovementInput(Dir);

	/*===============================================수정 필요===============================================*/
	// 발쪽, 몸통 살짝 위 전방 Trace.
	FVector FootStart = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	FVector FootEnd = FootStart + GetActorForwardVector() * (GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.5f);

	FVector BodyStart = GetActorLocation() + FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.5f);
	FVector BodyEnd = BodyStart + GetActorForwardVector() * (GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.5f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult Hit;

	bool bFootResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		FootStart,
		FootEnd,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		Hit,
		true);

	bool bBodyResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		BodyStart,
		BodyEnd,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		Hit,
		true);


	// 타겟과 벽을 사이에 두고있을 경우. 벽을 타고 올라가도록.
	if (bFootResult && bBodyResult)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		AddMovementInput(GetActorUpVector());
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

		FVector GroundCheckStart = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector GroundCheckCheckEnd = GroundCheckStart + GetActorUpVector() * (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * -0.1f);
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
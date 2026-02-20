// Copyright is owned by Veduy.

#include "MgbEnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "MgbPlayerCharacter.h"
#include "../MgbGameStateBase.h"
#include "../MgbGameplayTags.h"
#include "../MgbWeapon.h"
#include "../AbilitySystem/MgbAbilitySystemComponent.h"
#include "../AbilitySystem/AttributeSet/EnemyAttributeSet.h"
#include "../../Util/NetworkLog.h"

AMgbEnemyCharacter::AMgbEnemyCharacter()
{
	bReplicates = true;

	GetCapsuleComponent()->SetIsReplicated(true);
	GetCapsuleComponent()->SetCollisionProfileName(FName("Enemy"));
	GetCapsuleComponent()->SetCapsuleHalfHeight(80.f);
	GetCapsuleComponent()->SetCapsuleRadius(40.f);
	GetCapsuleComponent()->SetSimulatePhysics(false);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;

	GetMesh()->SetupAttachment(RootComponent);
	GetMesh()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.f, 0.0f));
	float Half = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -Half));
	GetMesh()->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->GroundFriction = 0.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 900.f, 0.f);
	GetCharacterMovement()->bIgnoreBaseRotation = true;

	EnemyAttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("EnemyAttributeSet"));

	AutoPossessAI = EAutoPossessAI::Disabled;
}

void AMgbEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	bSpawnFinished = false;
	AutoPossessAI = EAutoPossessAI::Disabled;

	auto MeshComp = GetMesh();
	if (MeshComp->GetSkeletalMeshAsset())
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

	//서버일때만
	if (!HasAuthority())
		return;

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMgbEnemyCharacter::CollisionHit);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		// 초기화
		ASC->InitAbilityActorInfo(this, this);
		
		// HP, Damage Attribute 적용.
		auto Level = Cast<AMgbGameStateBase>(GetWorld()->GetGameState())->PassedTimeSec;
		auto ContextHandle = ASC->MakeEffectContext();
		if (InitAttributeEffect)
		{
			auto EffectSpec = ASC->MakeOutgoingSpec(InitAttributeEffect, Level, ContextHandle);
			ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
		}
	}

	GetWorld()->GetTimerManager().SetTimer(CheckWallTimer, this, &AMgbEnemyCharacter::CheckWall, 0.5f, true, 2.f);

	auto Location = GetActorLocation();
	Location.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	SetActorLocation(Location);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TargetSpawnHeight = Location.Z + (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2);
}

void AMgbEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (bSpawnFinished == false)
		{
			float Height = FMath::Lerp(GetActorLocation().Z, TargetSpawnHeight, DeltaTime * 5.f);
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, Height), true);

			if (GetActorLocation().Z >= TargetSpawnHeight - 1.f)
			{
				bSpawnFinished = true;
				SpawnDefaultController();
				GetCapsuleComponent()->SetCollisionProfileName(FName("Enemy"));
			}

			return;
		}

		switch (CurrentMoveState)
		{
		case EMoveState::Idle:
			LookTarget(DeltaTime);
			break;
		case EMoveState::Walk:
			LookTarget(DeltaTime);
			MoveToTarget(DeltaTime);
			break;
		case EMoveState::Climb:
			LookTarget(DeltaTime);
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
	DOREPLIFETIME(AMgbEnemyCharacter, bSpawnFinished);
}

void AMgbEnemyCharacter::Destroyed()
{
	// 타이머 정리 (EndPlay보다 먼저 호출될 수 있으므로 명시적으로 정리)
	GetWorld()->GetTimerManager().ClearTimer(CheckWallTimer);

	if (HasAuthority())
	{
		if (auto GS = Cast<AMgbGameStateBase>(GetWorld()->GetGameState()))
		{
			GS->TotalKill++;
			GS->CurrentEnemyCount--;

			// 무기별 킬 기록 추가
			FName KillWeapon = NAME_None;
			if (LastDamageWeapon.IsValid())
			{
				KillWeapon = LastDamageWeapon->WeaponName;
			}
			GS->RecordWeaponKill(KillWeapon);
		}

		GetWorld()->SpawnActor<AActor>(XPCrystalClass, GetActorTransform());
	}

	Super::Destroyed();
}

void AMgbEnemyCharacter::SetLastDamageWeapon(AMgbWeapon* Weapon)
{
	LastDamageWeapon = Weapon;
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

	// 경사면에서 일정한 속도값을 위해서. 
	FVector Start = GetActorLocation();
	FVector End = Start + (GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * -1.3f);
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

		AddMovementInput(Dir);
	}
}

void AMgbEnemyCharacter::LookTarget(float DeltaTime)
{
	if (TargetActor)
	{
		FVector Dir = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		float Dot = FVector::DotProduct(Dir, FVector::UpVector);
		if (Dot > 0.9f || Dot < -0.9f)
		{
			return;
		}
		else
		{
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation());
			FRotator LookRotation = FRotator(GetActorRotation().Pitch, TargetRotation.Yaw, GetActorRotation().Roll);
			LookRotation = FMath::RInterpTo(GetActorRotation(), LookRotation, DeltaTime, 5.f);
			SetActorRotation(LookRotation);
		}
	}
}

void AMgbEnemyCharacter::CheckWall()
{
	if (!bSpawnFinished)
		return;
	
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
			CurrentMoveState = EMoveState::Climb;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		}
		else if (Cast<AMgbEnemyCharacter>(Hit.GetActor()))
		{
			CurrentMoveState = EMoveState::Climb;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		}
	}
	else if(!bResult && CurrentMoveState != EMoveState::Walk)
	{
		CurrentMoveState = EMoveState::Walk;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		if (Cast<AMgbEnemyCharacter>(GetCharacterMovement()->CurrentFloor.HitResult.GetActor()))
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}
}

void AMgbEnemyCharacter::ClimbWall(float DeltaTime)
{
	//서버일때만
	if (!HasAuthority())
		return;

	FVector Up = GetActorUpVector();
	FVector Dir = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
	Dir = (Dir + Up).GetSafeNormal();

	AddMovementInput(Dir * DeltaTime * 10.f);
}

void AMgbEnemyCharacter::CollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority())
		return;

	if (AMgbPlayerCharacter* Player = Cast<AMgbPlayerCharacter>(OtherActor))
	{
		if (Player->bDeath == true)
			return;

		// ApplyDamage By GameplayEffect
		UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent(); 
		//bool bInvincible = PlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Invincible")));
		bool bInvincible = PlayerASC->HasMatchingGameplayTag(TAG_Character_State_Invincible);
		if (bInvincible)
			return;

		// Knockback
		UCharacterMovementComponent* PlayerMove = Player->GetCharacterMovement();

		FVector ImpulseDir = Player->GetActorLocation() - GetActorLocation();
		ImpulseDir = ImpulseDir.GetSafeNormal();
		
		PlayerMove->AddImpulse(ImpulseDir * 15000.f);
		PlayerMove->AddImpulse(FVector(0.f, 0.f, 1.f) * 10000.f);

		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		if (HitDamageEffectClass)
		{
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(HitDamageEffectClass, 1.f, ContextHandle);
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), PlayerASC);
		}
	}
}

void AMgbEnemyCharacter::HandleDamageEffect_Implementation()
{

}

FVector AMgbEnemyCharacter::GetPredictedTargetLocation(float PredictionTime) const
{
	if (!IsValid(TargetActor))
	{
		return FVector::ZeroVector;
	}

	FVector CurrentLocation = TargetActor->GetActorLocation();

	// Target의 Velocity를 가져옴 (Character인 경우 CharacterMovement 사용)
	FVector TargetVelocity = FVector::ZeroVector;
	if (ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor))
	{
		TargetVelocity = TargetCharacter->GetCharacterMovement()->Velocity;
	}

	// 예상 위치 = 현재 위치 + (속도 * 예측 시간)
	FVector PredictedLocation = CurrentLocation + (TargetVelocity * PredictionTime);

	return PredictedLocation;
}

void AMgbEnemyCharacter::SpawnAttackTelegraphAtPredictedTarget(TSubclassOf<AActor> TelegraphActorClass, float PredictionTime)
{
	// 서버에서만 실행
	if (!HasAuthority())
	{
		return;
	}

	if (!IsValid(TargetActor) || !TelegraphActorClass)
	{
		return;
	}

	// 예상 위치 계산
	FVector PredictedLocation = GetPredictedTargetLocation(PredictionTime);

	FVector TraceStart = PredictedLocation + FVector(0.f, 0.f, 5000.f);
	FVector TraceEnd = PredictedLocation + FVector(0.f, 0.f, -5000.f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (TargetActor)
	{
		QueryParams.AddIgnoredActor(TargetActor);
	}

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_WorldStatic,
		QueryParams
	);

	FVector SpawnLocation = bHit ? HitResult.ImpactPoint : PredictedLocation;

	// Telegraph 액터를 바닥 위치에 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	AActor* Telegraph = GetWorld()->SpawnActor<AActor>(
		TelegraphActorClass,
		SpawnLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (Telegraph)
	{
		// 추가 설정이 필요하면 여기에 작성
	}
}

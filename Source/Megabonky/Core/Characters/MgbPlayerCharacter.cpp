// Copyright is owned by Veduy.


#include "MgbPlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Algo/Sort.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MgbEnemyCharacter.h"
#include "../AbilitySystem/MgbAbilitySystemComponent.h"
#include "../AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "../MgbWeapon.h"
#include "../../Actors/MgbItemActor.h"

#include "../../Util/NetworkLog.h"

AMgbPlayerCharacter::AMgbPlayerCharacter()
{
	bReplicates = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(80.f);
	GetCapsuleComponent()->SetCapsuleRadius(40.f);

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->GroundFriction = 0.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 900.f, 0.f);
	GetCharacterMovement()->bIgnoreBaseRotation = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.f, 0.0f));
	float Half = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -Half));
	GetMesh()->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 0.f;
	SpringArm->CameraRotationLagSpeed = 15.f;
	SpringArm->CameraLagMaxDistance = 0.f;
	SpringArm->bClampToMaxPhysicsDeltaTime = false;
	SpringArm->bUseCameraLagSubstepping = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 1500.f;

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MainCamera->SetupAttachment(SpringArm);
	
	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRange"));
	PickupSphere->SetupAttachment(RootComponent);
	PickupSphere->SetSphereRadius(300.f);
	PickupSphere->SetCollisionProfileName(FName("PickupDetector"));

	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
}

void AMgbPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	MeshBaseQuat = GetMesh()->GetRelativeRotation().Quaternion();

	if (HasAuthority())
	{
		PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &AMgbPlayerCharacter::PickupBeginOverlap);
	}
}

void AMgbPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCharacterMeshRotation(DeltaTime);
}

void AMgbPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);
	
		auto ContextHandle = ASC->MakeEffectContext();
		auto SpecHandle = ASC->MakeOutgoingSpec(InitAttributeEffect, 1.f, ContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
} 

void AMgbPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMgbPlayerCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMgbPlayerCharacter::Look);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	}
}

void AMgbPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMgbPlayerCharacter, Tomes);
	DOREPLIFETIME(AMgbPlayerCharacter, Weapons);
}

bool AMgbPlayerCharacter::FindPrimaryTargetByCondition(AActor*& OutPrimaryTarget)
{
	// 일정 범위내에 액터들 중에서 거리만 판별해서, 가까운 3마리중 타겟 랜덤
	FVector Origin = GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> Hits;
	TArray<AActor*> OutActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Origin, 2000.f,
		ObjectTypes, AMgbEnemyCharacter::StaticClass(), 
		ActorsToIgnore, OutActors);


	if (!OutActors.IsEmpty())
	{
		// 거리 오름차순(가까운순) 정렬 후 근거리 3마리에서 랜덤으로 PrimaryTargetSetting;
		Algo::Sort(
			OutActors,
			[&](const AActor* A, const AActor* B)
			{
				return FVector::DistSquared(A->GetActorLocation(), Origin) < FVector::DistSquared(B->GetActorLocation(), Origin);
			});

		AActor* Targets[3] = { nullptr };
		uint8 TargetCount = FMath::Min(OutActors.Num(), 3);
		for (uint8 i = 0; i < TargetCount; ++i)
		{	
			OutActors.IsValidIndex(i) ? Targets[i] = OutActors[i] : nullptr;
		}
		OutPrimaryTarget = Targets[FMath::RandRange(0, TargetCount - 1)];
		return true;

		// 최단 거리 액터
		/*float ClosesetDistance = MAX_FLT;
		for (const auto& Target : OutActors)
		{
			float Distance = (Target->GetActorLocation() - Origin).Length();
			if (Distance < ClosesetDistance)
			{	
				ClosesetDistance = Distance;
				OutPrimaryTarget = Target;
			}
		}*/
	}

	return false;
}

void AMgbPlayerCharacter::EquipWeapon(TSubclassOf<AMgbWeapon> NewWeapon)
{
	if (!HasAuthority())
	{
		return;
	}

	if (Weapons.Num() >= 4)
	{
		return;
	}

	if (NewWeapon)
	{
		AMgbWeapon* SpawnedWeapon = Cast<AMgbWeapon>(GetWorld()->SpawnActor(NewWeapon));
		SpawnedWeapon->SetOwner(this);

		Weapons.Add(SpawnedWeapon);
	}
}

void AMgbPlayerCharacter::ActivateWeaponsAbility()
{
	if (bDeath)
		return;

	// 모든 Weapon의 어빌리티 Activate
	// PlayerAttackSpeed = 100% ~ x%;  Interval 2초,  (2 / PlayerAttackSpeed / 100)
	float PlayerAttackSpeed = GetAbilitySystemComponent()->GetNumericAttribute(UPlayerAttributeSet::GetAttackSpeedAttribute());
	GetWorld()->GetTimerManager().SetTimer(ActivateAbilityHandle,
		[this]()
		{
			for (const auto& Weapon : Weapons)
			{
				Weapon->GetAbilitySystemComponent()->TryActivateAbilityByClass(Weapon->AbilityClass);
			}
		},
		2 / (PlayerAttackSpeed / 100),
		true,
		1.f);
}

void AMgbPlayerCharacter::UpdateCharacterMeshRotation(float DeltaTime)
{
	const FVector Start = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	const FVector End = Start + (GetActorUpVector() * -500.f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	//ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;

	const bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		Start, End,
		ObjectTypes, false,
		ActorsToIgnore, EDrawDebugTrace::Type::ForOneFrame, Hit,
		true);

	if (bResult)
	{	
		//FVector FloorNormal = Hit.Normal;

		//FRotator RotationXZ = UKismetMathLibrary::MakeRotFromXZ(GetActorForwardVector(), FloorNormal);
		//FRotator RotationYZ = UKismetMathLibrary::MakeRotFromYZ(GetActorRightVector(), FloorNormal);

		//FRotator CurrentRotation = GetMesh()->GetRelativeRotation();

		//FRotator TargetRotation;
		//TargetRotation.Pitch = RotationYZ.Pitch;
		//TargetRotation.Roll = RotationXZ.Roll;

		//// Mesh가 기본으로 Yaw축으로 -90도 돌아가있어서 
		//CurrentRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetRotation.Roll, DeltaTime, 5.f);
		//CurrentRotation.Roll = FMath::FInterpTo(CurrentRotation.Roll, -TargetRotation.Pitch, DeltaTime, 5.f);

		//GetMesh()->SetRelativeRotation(CurrentRotation);

		const FVector FloorNormal = Hit.ImpactNormal;
		
		FVector WorldSlopeForward = FVector::VectorPlaneProject(GetActorForwardVector(), FloorNormal);
		WorldSlopeForward = WorldSlopeForward.GetSafeNormal();

		const FQuat WorldSlopeQuat = FRotationMatrix::MakeFromZY(FloorNormal, WorldSlopeForward).ToQuat();

		const FQuat ActorWorldQuat = GetActorQuat();

		// 경사 회전을 Actor기준으로 
		FQuat SlopeLocalQuat = ActorWorldQuat.Inverse() * WorldSlopeQuat;

		//Yaw 제거
		FQuat SlopeNoYawQuat;
		{
			FRotator R = SlopeLocalQuat.Rotator();
			R.Yaw = 0.f;
			SlopeNoYawQuat = R.Quaternion();
		}

		//기본 Mesh 회전과 합성 (Quat 곱)
		const FQuat TargetMeshQuat = MeshBaseQuat * SlopeNoYawQuat;

		const FQuat Current = GetMesh()->GetRelativeRotation().Quaternion();

		//Interp 계열은 각도값 보간이라 복합 회전에 약하고, Slerp는 회전 자체를 보간해서 경사면·메쉬 정렬에 안정적이다.
		const FQuat Smooth = FQuat::Slerp(Current, TargetMeshQuat, DeltaTime * 10.f);
		GetMesh()->SetRelativeRotation(Smooth);
	}
	else
	{
		FRotator CurrentRotation = GetMesh()->GetRelativeRotation();

		CurrentRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, 0, DeltaTime, 10.f);
		CurrentRotation.Roll = FMath::FInterpTo(CurrentRotation.Roll, 0, DeltaTime, 10.f);

		GetMesh()->SetRelativeRotation(CurrentRotation);
	}
}

void AMgbPlayerCharacter::HandleDeath()
{
	bDeath = true;

	if (ActivateAbilityHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ActivateAbilityHandle);
	}

	DeathFadeOut();
}

void AMgbPlayerCharacter::PickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (AMgbItemActor* Item = Cast<AMgbItemActor>(OtherActor))
		{
			Item->OnAction.Broadcast(this);
		}
	}
}

void AMgbPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);

	FVector ForwardDir = UKismetMathLibrary::GetForwardVector(YawRotation);
	FVector RightDir = UKismetMathLibrary::GetRightVector(YawRotation);

	AddMovementInput(ForwardDir * Input.X + RightDir * Input.Y);
}

void AMgbPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	AddControllerPitchInput(Input.X);
	AddControllerYawInput(Input.Y);
}

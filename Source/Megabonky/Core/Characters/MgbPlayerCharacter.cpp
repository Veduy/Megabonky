// Copyright is owned by Veduy.


#include "MgbPlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
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
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 900.f);
	
	
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

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MainCamera->SetupAttachment(SpringArm);
	
	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
}

void AMgbPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMgbPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMgbPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);
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

		// 최단 거리 액터 찾는거.
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
	if (NewWeapon)
	{
		AMgbWeapon* SpawnedWeapon = Cast<AMgbWeapon>(GetWorld()->SpawnActor(NewWeapon));
		SpawnedWeapon->SetOwner(this);
		Weapons.Add(SpawnedWeapon);
	}
}

void AMgbPlayerCharacter::ActivateWeaponsAbility()
{
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

// Copyright is owned by Veduy.


#include "MgbPlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../AbilitySystem/MgbAbilitySystemComponent.h"
#include "../AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "../MgbWeapon.h"

#include "../../Util/NetworkLog.h"

AMgbPlayerCharacter::AMgbPlayerCharacter()
{
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.f, 0.0f));
	float Half = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -Half));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 0.f;
	SpringArm->CameraRotationLagSpeed = 10.f;
	SpringArm->CameraLagMaxDistance = 0.f;

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MainCamera->SetupAttachment(SpringArm);

	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
}

void AMgbPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
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

}

bool AMgbPlayerCharacter::FindPrimaryTargetByCondition(AActor*& OutPrimaryTarget)
{
	// 일정 범위내에 액터들 중에서 거리만 판별해서, 최단거리 액터를 타겟으로 설정.
	FVector Start = GetActorLocation();
	FVector End = Start;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> Hits;

	bool bResult = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
		Start, End, 1000.f, ObjectTypes, false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration, Hits, true, FLinearColor::Red, FLinearColor::Green, 2.f);

	if (bResult)
	{
		float ClosestDistance = MAX_FLT;
		for (const auto& Hit : Hits)
		{
			if (Hit.Distance < ClosestDistance)
			{
				ClosestDistance = Hit.Distance;
				OutPrimaryTarget = Hit.GetActor();
			}
		}
		return true;
	}

	return false;
}

void AMgbPlayerCharacter::SpawnDefaultWeapon()
{
	if (DefaultWeaponClass)
	{
		AMgbWeapon* SpawnedWeapon = Cast<AMgbWeapon>(GetWorld()->SpawnActor(DefaultWeaponClass));
		SpawnedWeapon->SetOwner(this);
		Weapons.Add(SpawnedWeapon);
	}
}

void AMgbPlayerCharacter::ActivateWeaponsAbility()
{
	// 모든 Weapon의 어빌리티 Activate
	float AttackSpeed = GetAbilitySystemComponent()->GetNumericAttribute(UPlayerAttributeSet::GetAttackSpeedAttribute());
	// 	UE_API void GetAllAbilities(TArray<FGameplayAbilitySpecHandle>& OutAbilityHandles) const;
	GetWorld()->GetTimerManager().SetTimer(ActivateAbilityHandle,
		[this]()
		{
			for (const auto& Weapon : Weapons)
			{
				Weapon->GetAbilitySystemComponent()->TryActivateAbilityByClass(Weapon->AbilityClass);
			}
		},
		2.f,	//(AttackSpeed / 100.f) + 1,
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

// Copyright is owned by Veduy.


#include "MgbPlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../AbilitySystem/MgbAbilitySystemComponent.h"
#include "../AbilitySystem/AttributeSet/CharacterAttributeSet.h"
#include "../AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "../AbilitySystem/AttributeSet/WeaponAttributeSet.h"

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

	CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
	WeaponAttributeSet = CreateDefaultSubobject<UWeaponAttributeSet>(TEXT("WeaponAttributeSet"));
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

void AMgbPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);
	}
	else if (!ASC)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Ability System is Null"));
	}
}

void AMgbPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	// <TEST> 모든 어빌리티 Activate
	float AttackSpeed = GetAbilitySystemComponent()->GetNumericAttribute(UPlayerAttributeSet::GetAttackSpeedAttribute());
	NET_LOG(FString::Printf(TEXT("AttackSpeed: %f"), AttackSpeed));

	// 	UE_API void GetAllAbilities(TArray<FGameplayAbilitySpecHandle>& OutAbilityHandles) const;
	FTimerHandle ActivateAbilityHandle;
	GetWorld()->GetTimerManager().SetTimer(ActivateAbilityHandle,
		[this]()
		{
			GetAbilitySystemComponent()->GetAllAbilities(Abilities);

			if (!Abilities.IsEmpty())
			{
				for (const auto& Ability : Abilities)
				{
					GetAbilitySystemComponent()->TryActivateAbility(Ability);
				}
			}
		},
		AttackSpeed,
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

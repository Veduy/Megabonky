// Copyright is owned by Veduy.


#include "MgbPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "MgbGameStateBase.h"
#include "MgbGameModeBase.h"
#include "MgbWeapon.h"
#include "Characters/MgbPlayerCharacter.h"
#include "Data/WeaponInfo.h"
#include "MgbGameplayTags.h"
#include "../UI/InGame/InGame.h"
#include "../Actors/DamageTextActor.h"
#include "../Util/NetworkLog.h"

AMgbPlayerController::AMgbPlayerController()
{
	ConstructorHelpers::FClassFinder<AActor>BP_DamageTextActor(TEXT("/Game/Blueprints/Actors/BP_DamageTextActor.BP_DamageTextActor_C"));
	if (BP_DamageTextActor.Succeeded())
	{
		DamageTextActorClass = BP_DamageTextActor.Class;
	}
}

void AMgbPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);


}

void AMgbPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AMgbPlayerController::OnPossess(APawn* aPawn)
{
	// 오직 서버에서만 호출됨.
	Super::OnPossess(aPawn);
	
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
	AMgbPlayerCharacter* PlayerCharacter = Cast<AMgbPlayerCharacter>(aPawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->EquipWeapon(PlayerCharacter->DefaultWeaponClass);
		PlayerCharacter->ActivateWeaponsAbility();
	}
}

void AMgbPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMgbPlayerController::ServerResumeRequestCountIncrementAndCheck_Implementation()
{
	AMgbGameStateBase* GS = Cast<AMgbGameStateBase>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->HandleResumeRequest();
	}
}

void AMgbPlayerController::ServerApplyWeaponUpgradeEffect_Implementation(const TArray<FWeaponUpgradeOption>& UpgradeData)
{
	AMgbPlayerCharacter* MgbPlayer = Cast<AMgbPlayerCharacter>(GetPawn());
	UAbilitySystemComponent* PlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MgbPlayer);

	// 무기도 업글할 무기를 찾거나, 없다면 무기를 추가시키는 로직 필요.
	AMgbWeapon* MgbWeapon = MgbPlayer->Weapons[0]; // 임시로 0번 무기만 적용.
	UAbilitySystemComponent* WeaponASC = MgbWeapon->GetAbilitySystemComponent();

	auto ContextHandle = WeaponASC->MakeEffectContext();
	auto SpecHandle = WeaponASC->MakeOutgoingSpec(GE_WeaponUpgradeDefaultClass, 1.f, ContextHandle);
	check(GE_WeaponUpgradeDefaultClass);
	auto EffectSpec = SpecHandle.Data.Get();

	EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_Damage, 0.f);
	EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_CritChance, 0.f);
	EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_CritDamage, 0.f);
	EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_ProjectileCount, 0.f);
	EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_ProjectileSpeed, 0.f);
	EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_ProjectileBounces, 0.f);
	EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_Size, 0.f);
	EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_Knockback, 0.f);
	EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_Duration, 0.f);

	for (const auto& u : UpgradeData)
	{
		switch (u.StatType)
		{
		case EWeaponUpgradeStat::Damage:
			EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_Damage, u.IncreaseValue);
			break;
		case EWeaponUpgradeStat::CritChance:
			EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_CritChance, u.IncreaseValue);
			break;
		case EWeaponUpgradeStat::CritDamage:
			EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_CritDamage, u.IncreaseValue);
			break;
		case EWeaponUpgradeStat::ProjectileCount:
			EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_ProjectileCount, u.IncreaseValue);
			break;
		case EWeaponUpgradeStat::ProjectileSpeed:
			EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_ProjectileSpeed, u.IncreaseValue);
			break;
		case EWeaponUpgradeStat::ProjectileBounce:
			EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_ProjectileBounces, u.IncreaseValue);
			break;
		case EWeaponUpgradeStat::Size:
			EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_Size, u.IncreaseValue);
			break;
		case EWeaponUpgradeStat::Knockback:
			EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_Knockback, u.IncreaseValue);
			break;
		case EWeaponUpgradeStat::Duration:
			EffectSpec->SetSetByCallerMagnitude(TAG_Attribute_Weapon_Duration, u.IncreaseValue);
			break;
		}
	}

	WeaponASC->ApplyGameplayEffectSpecToSelf(*EffectSpec);

	//배열 초기화 라기보다는 위젯 전체 초기화 해줘야할
	//Upgrades;
}

void AMgbPlayerController::ClientSpawnDamageTextActor_Implementation(FVector Location, float DamageValue)
{
	FTransform SpawnTransform = FTransform(FRotator(), Location, FVector::OneVector);
	AActor* Actor = GetWorld()->SpawnActor<AActor>(DamageTextActorClass, SpawnTransform);

	if (Actor)
	{
		Cast<ADamageTextActor>(Actor)->SetText(DamageValue);
	}
}

void AMgbPlayerController::GenerateUpgradeInfo()
{
	// 서버에서 호출되야함.		
	AMgbGameStateBase* GS = Cast<AMgbGameStateBase>(GetWorld()->GetGameState());
	TArray<FName> WeaponNames = GS->DT_Weapon->GetRowNames();
	
	// 나중에 장비한 장비인경우엔, 더 높은 확률로 나오게 수정가능할듯.
	WeaponNames.Sort([](const auto&, const auto&)
	{
		return FMath::RandBool();
	});

	bool bWeapon = true;
	for (int SlotNum = 0; SlotNum < 3; SlotNum++)
	{
		// 2분의 1확률로 무기/비전서.
		// 일단 무조건 무기.
		// 무기도 겹쳐서 나오면 안됨;
		// 무기 종류도 임시배열에 저장후 정렬 후 앞에서 3개뽑는 순으로 가야겠네;
		//무기 칸이 남아있다면 데이터테이블에 있는 무기들중에서 랜덤으로 선택. >> (가지고있는 무기에 추가 확률 부여)
		//무기 칸이 없다면, 플레이어가 가지고 있는 무기들 중에서 선택.
		if (bWeapon)
		{
			FName RandomWeaponName = WeaponNames[SlotNum];

			// 업그레이드할 무기는 정해짐.
			FMgbWeaponInfo* WeaponInfo = GS->DT_Weapon->FindRow<FMgbWeaponInfo>(RandomWeaponName, FString("Find Weapon"));
			if (!WeaponInfo)
			{
				return;
			}

			auto WeaponBonus = GS->DT_WeaponUpgradeBonus->FindRow<FMgbWeaponUpgradeBonus>(RandomWeaponName, FString("Find Bouns"));
			if (!WeaponBonus)
			{
				return;
			}

			auto Temp = WeaponBonus->UpgradeOptions;
			TArray<FWeaponUpgradeOption> SelectedOptions;

			// 섞기
			Temp.Sort([](const auto&, const auto&)
			{
				return FMath::RandBool();
			});

			int32 Count = FMath::RandRange(1, 2);

			for (int32 i = 0; i < Count && i < Temp.Num(); ++i)
			{
				SelectedOptions.Add(Temp[i]);
			}

			InGameWidget->SetItemUpgradeSlot(SlotNum, RandomWeaponName, SelectedOptions);
		}
		else
		{

		}
	}
}
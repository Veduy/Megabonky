// Copyright is owned by Veduy.


#include "MgbPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/AudioComponent.h"
#include "Algo/RandomShuffle.h"

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

	Audio_Levelup = CreateDefaultSubobject<UAudioComponent>("LevelUp");
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

void AMgbPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 맵 이동(ServerTravel) 시 PlayerController의 타이머 안전하게 정리
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::EndPlay(EndPlayReason);
}

void AMgbPlayerController::ServerResumeRequestCountIncrementAndCheck_Implementation()
{
	AMgbGameStateBase* GS = Cast<AMgbGameStateBase>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->HandleResumeRequest();
	}
}

void AMgbPlayerController::ServerApplyWeaponUpgradeEffect_Implementation(FName InWeaponName, const TArray<FWeaponUpgradeOption>& UpgradeData)
{
	AMgbPlayerCharacter* MgbPlayer = Cast<AMgbPlayerCharacter>(GetPawn());
	UAbilitySystemComponent* PlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MgbPlayer);

	auto EquipedWeapons = MgbPlayer->Weapons;
	for (const auto& w : EquipedWeapons)
	{
		if (w->WeaponName == InWeaponName)
		{
			UAbilitySystemComponent* WeaponASC = w->GetAbilitySystemComponent();

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
			return;
		}
	}

	if (DT_Weapons)
	{
		auto WeaponInfo = DT_Weapons->FindRow<FMgbWeaponInfo>(InWeaponName, FString("Find Weapon"));

		TSubclassOf<AMgbWeapon> Weapon = WeaponInfo->WeaponClass;
		if (Weapon)
		{
			MgbPlayer->EquipWeapon(Weapon);
		}
	}
}

void AMgbPlayerController::ClientSpawnDamageTextActor_Implementation(FVector Location, float DamageValue, bool bCrit)
{
	FTransform SpawnTransform = FTransform(FRotator(), Location, FVector::OneVector);
	AActor* Actor = GetWorld()->SpawnActor<AActor>(DamageTextActorClass, SpawnTransform);

	if (Actor)
	{
		auto DmgText = Cast<ADamageTextActor>(Actor);
		if (DmgText)
		{
			DmgText->SetText(DamageValue);
			if (bCrit)
			{
				DmgText->SetCritText();
			}
		}
	}
}

TArray<FUpgradeSlotInfo> AMgbPlayerController::ServerGenerateUpgradeSlots()
{
	TArray<FUpgradeSlotInfo> Result;

	AMgbGameStateBase* GS = Cast<AMgbGameStateBase>(GetWorld()->GetGameState());
	AMgbPlayerCharacter* MgbPlayer = Cast<AMgbPlayerCharacter>(GetPawn());
	TArray<FName> WeaponNames;

	int WeaponCount = MgbPlayer->Weapons.Num();
	if (WeaponCount < 4)
	{
		WeaponNames = GS->DT_Weapon->GetRowNames();
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			WeaponNames.Add(MgbPlayer->Weapons[i]->WeaponName);
		}
	}

	Algo::RandomShuffle(WeaponNames);

	for (int SlotNum = 0; SlotNum < 3; SlotNum++)
	{
		FName RandomWeaponName = WeaponNames[SlotNum];
		FMgbWeaponInfo* WeaponInfo = GS->DT_Weapon->FindRow<FMgbWeaponInfo>(RandomWeaponName, FString("Find Weapon"));
		if (!WeaponInfo)
		{
			return Result;
		}

		auto WeaponBonus = GS->DT_WeaponUpgradeBonus->FindRow<FMgbWeaponUpgradeBonus>(RandomWeaponName, FString("Find Bonus"));
		if (!WeaponBonus)
		{
			return Result;
		}

		auto Temp = WeaponBonus->UpgradeOptions;
		TArray<FWeaponUpgradeOption> SelectedOptions;
		Algo::RandomShuffle(Temp);

		int32 Count = FMath::RandRange(1, 2);
		for (int32 i = 0; i < Count && i < Temp.Num(); ++i)
		{
			SelectedOptions.Add(Temp[i]);
		}

		FUpgradeSlotInfo SlotInfo;
		SlotInfo.WeaponName = RandomWeaponName;
		SlotInfo.Options = SelectedOptions;
		Result.Add(SlotInfo);
	}

	return Result;
}

void AMgbPlayerController::ClientShowUpgradeWindow_Implementation(const TArray<FUpgradeSlotInfo>& Slots)
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		InGameWidget->SetItemUpgradeSlot(i, Slots[i].WeaponName, Slots[i].Options);
	}
	InGameWidget->ShowItemSelectWindow();
}
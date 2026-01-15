// Copyright is owned by Veduy.


#include "ItemSelectButton.h"
#include "Components/Button.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "../../Core/MgbGameplayTags.h"
#include "../../Core/Characters/MgbPlayerCharacter.h"
#include "../../Core/MgbWeapon.h"
#include "../../Util/NetworkLog.h"

void UItemSelectButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UItemSelectButton::HandleButtonClicked);
	}
}

void UItemSelectButton::HandleButtonClicked()
{
	// PlayerController에서 Server함수로 서버에 요청해서 서버에서 Effect적용시키는 방식이 이 좋을듯? 
	APlayerController* PC = GetOwningPlayer();
	
	AMgbPlayerCharacter* MgbPlayer = Cast<AMgbPlayerCharacter>(PC->GetPawn());
	UAbilitySystemComponent* PlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PC->GetPawn()); 

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
			
	for (const auto& u : Upgrades)
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

	OnItemSelected.Broadcast();
}

// Copyright is owned by Veduy.

#pragma once

#include "NativeGameplayTags.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_State_Jump);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_State_Invincible);


/// <summary>
/// 무기 / 비전서
/// if( 현재 장착한 장비 개수 < 장착가능한 장비 개수)
/// { 현재 장착한 장비 + 다른거 }
/// else (현재 장착한 장비 개수 = 최대치)
/// { 현재 장착한 장비 중에서만 고르게 }
/// 
///  
/// 
/// </summary>
//UE_DECLARE_G

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Weapon_FireStaff);

// GE_WeaponUpgrade 적용시킬때, SetbyCaller로 값 설정해줄건데, 태그가 필요함
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Weapon_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Weapon_CritChance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Weapon_CritDamage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Weapon_ProjectileCount);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Weapon_ProjectileSpeed);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Weapon_ProjectileBounces);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Weapon_Size);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Weapon_Knockback);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attribute_Weapon_Duration);

// GameplayCue
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Lightning_Hit);

// SetByCaller 태그
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_AreaDamageMultiplier);

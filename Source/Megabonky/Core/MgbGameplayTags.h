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
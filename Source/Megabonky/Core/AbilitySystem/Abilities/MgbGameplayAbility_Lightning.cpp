// Copyright is owned by Veduy.


#include "MgbGameplayAbility_Lightning.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

#include "../AttributeSet/WeaponAttributeSet.h"
#include "../AttributeSet/PlayerAttributeSet.h"
#include "../../MgbWeapon.h"
#include "../../MgbGameplayTags.h"

void UMgbGameplayAbility_Lightning::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Lightning();
}

void UMgbGameplayAbility_Lightning::Lightning()
{
	/* Info Caching*/
	UAbilitySystemComponent* WeaponASC = GetAbilitySystemComponentFromActorInfo();

	GetActorInfo();
	AActor* WeaponActor = GetCurrentActorInfo()->AvatarActor.Get();

	// PlayerActor
	AActor* PlayerActor = WeaponActor->GetOwner();

	// Weapon ProjectileCount + Player ProjectileCount
	uint8 WeaponProjectileCount = WeaponASC->GetNumericAttribute(UWeaponAttributeSet::GetProjectileCountAttribute());

	UAbilitySystemComponent* PlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerActor);
	uint8 PlayerProjectileCount = PlayerASC->GetNumericAttribute(UPlayerAttributeSet::GetProjectileCountAttribute());

	LightingCount = WeaponProjectileCount + PlayerProjectileCount;

	/*Find Targets*/
	TArray<AActor*>ActorsToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	TArray<FHitResult>OutHits;
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
		PlayerActor->GetActorLocation(), PlayerActor->GetActorLocation(), 1500.f,
		ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true);

	TArray<AActor*> TargetActors;
	for (const auto& hit : OutHits)
	{
		TargetActors.Add(hit.Component->GetOwner());
		if (TargetActors.Num() >= LightingCount)
		{
			break;
		}
	}

	/*Apply Damage Effect*/
	AMgbWeapon* MgbWeapon = Cast<AMgbWeapon>(WeaponActor);
	if (MgbWeapon)
	{
		FGameplayEffectContextHandle EffectContextHandle = WeaponASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(WeaponActor);
		EffectContextHandle.AddInstigator(WeaponActor, WeaponActor);

		FGameplayEffectSpecHandle EffectSpecHandle = WeaponASC->MakeOutgoingSpec(MgbWeapon->DamageEffectClass, 1.f, EffectContextHandle);
		EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(TAG_Data_AreaDamageMultiplier, 1.f);

		float InterverTime = LightingCount > 0 ? 1.f / LightingCount : 1.f;
		if (TargetActors.Num() > 0 && MgbWeapon)
		{
			TSharedPtr<FTimerHandle> LightingTimerHandle = MakeShared<FTimerHandle>();
			TSharedPtr<int32>TargetIndex = MakeShared<int32>(0);

			GetWorld()->GetTimerManager().SetTimer(
				*LightingTimerHandle,
				[this, WeaponASC, EffectSpecHandle, TargetActors, LightingTimerHandle, TargetIndex]()
				{
					if (!WeaponASC || !EffectSpecHandle.IsValid())
					{
						if (LightingTimerHandle.IsValid())
						{
							GetWorld()->GetTimerManager().ClearTimer(*LightingTimerHandle);
						}
						return;
					}

					if (*TargetIndex >= TargetActors.Num())
					{
						if (LightingTimerHandle.IsValid())
						{
							GetWorld()->GetTimerManager().ClearTimer(*LightingTimerHandle);
						}
						return;
					}

					AActor* Target = TargetActors[*TargetIndex];
					if (Target)
					{
						auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
						if (TargetASC)
						{
							WeaponASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);

							float height = Cast<ACharacter>(Target)->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
							FVector TargetLoc = Cast<ACharacter>(Target)->GetActorLocation();
							TargetLoc.Z -= height;

							FGameplayCueParameters Params;
							Params.Location = TargetLoc;
							TargetASC->ExecuteGameplayCue(TAG_GameplayCue_Lightning_Hit, Params);
						}
					}

					(*TargetIndex)++;
				},
				InterverTime,
				true,
				0.f);
		}
	}


}
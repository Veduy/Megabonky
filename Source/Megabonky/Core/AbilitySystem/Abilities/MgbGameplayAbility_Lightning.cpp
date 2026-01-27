// Copyright is owned by Veduy.


#include "MgbGameplayAbility_Lightning.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "../AttributeSet/WeaponAttributeSet.h"
#include "../AttributeSet/PlayerAttributeSet.h"
#include "../../MgbWeapon.h"

void UMgbGameplayAbility_Lightning::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

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

		for (const auto& target : TargetActors)
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(target);
			WeaponASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UMgbGameplayAbility_Lightning::Lightning()
{

}